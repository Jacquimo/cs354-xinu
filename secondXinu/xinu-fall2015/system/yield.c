/* yield.c - yield */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  yield  -  Voluntarily relinquish the CPU (end a timeslice)
 *------------------------------------------------------------------------
 */
syscall	yield(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();

	// upgrade process priority because it is voluntarily gave up CPU
	upgradeProcPrio();

	resched();
	restore(mask);
	return OK;
}
