#include <xinu.h>
#include <stdio.h>

extern int stackdepth(void);

void printnoloop(char val) {
	//putchar(val);
	fprintf(stdout, "\nPrintNoLoop \"%c\" stack depth: %d\n", val, stackdepth());
	//fflush(stdout);
}

