/* Lab 01 app */

#include <xinu.h>
#include <stdio.h>

process lab0app1(void) {
	kprintf("\nVery lab0app1. Much message. Such PID=%d\n", getpid());
	return OK;
}
