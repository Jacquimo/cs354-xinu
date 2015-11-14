#include <xinu.h>
#include <stdio.h>

unsigned long *esp;
unsigned long *ebp;

int stackdepth() {
	struct procent *proc = &proctab[currpid];
	unsigned long *top_esp, *top_ebp;
	char *base = proc->prstkbase;
	int count = 0;

	asm("movl %esp, esp");
	asm("movl %ebp, ebp");
	top_ebp = ebp;
	top_esp = esp;

	//printf("\n\nebp: 0x%08X\t\tesp: 0x%08X\t\tbottom: 0x%08X\n",
	//	top_ebp, top_esp, base);

	//printf("\n\n\nBase: 0x%08X\n\n", base);
	//printf("\n\n");
	do {
		count++;
		printf("\nNumber of frames: %d\n", count);
		printf("ebp: 0x%08X\t\tesp: 0x%08X\n", top_ebp, top_esp);
		printf("Length of Frame: %d bytes\n\n", (uint32)top_ebp - (uint32)top_esp);
		
		top_esp = top_ebp + 1;
		top_ebp = (unsigned long *)*(top_ebp);
	} while ((uint32)base > (uint32)top_ebp);

	return count;
}

