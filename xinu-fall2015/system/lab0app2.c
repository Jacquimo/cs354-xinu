/* Lab 02 app */

#include <xinu.h>
#include <stdio.h>

process lab0app2(void) {
	kprintf("\n3782 * 751 = %d\t\tPID=%d\n", 3782 * 751, getpid());
	return OK;
}
