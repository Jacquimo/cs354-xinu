/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

qid16	multiqueue[NUM_PRIO_LVLS];	/* Multilevel Feedback Queue */

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;

	// enqueue process into the queue associated with their priority
	enqueue(pid, multiqueue[prptr->prprio]);

	resched();

	return OK;
}
