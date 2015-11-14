/* myvictim.c - myvictim */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  myvictim  -  a process create by main
 *------------------------------------------------------------------------
 */

extern int myvictimglobal;
extern syscall stacktrace(int);

void second() {
	int l = 8 - 6;
	
	sleepms(l * 1000);
	#ifdef VERBOSE
	stacktrace(currpid);
	#endif
}

void first() {
	int j = 5 + 7;
	j /= 6;
	second();
}

void myvictim() {
	first();
	printf("\nPID = %d\t\tMy Victim: myvictimglobal = %d\n", currpid, myvictimglobal);
}

