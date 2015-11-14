#include <xinu.h>
#include <stdio.h>

#define INNERB 5000000
#define OUTERB 5

void printloop(char val) {
	int i = 0;
	for (; i < OUTERB; ++i) {
		putchar(val);
		int j = 0;
		for (; j < INNERB; ++j) {

		}
	}
}

