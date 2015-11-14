/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	int i = 0;

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	// find the queue with highest priority that has ready processes
	for (i = NUM_PRIO_LVLS - 1; i >= 0; --i) {
		if (nonempty(multiqueue[i]))
			break;
	}

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > i)
			return;

		/* Old process will no longer remain current */
		ptold->prstate = PR_READY;

		// don't add the null process to a queue; it will only be called when all queues are empty
		if (currpid != 0)
			// enqueue process into the appropriate priority level/queue
			enqueue(currpid, multiqueue[ptold->prprio]);
	}

	/* Force context switch to highest priority ready process */
	// the highest priority level with processes ready is i

	// if i < 0, there are no non-null ready processes, so we schedule the null process
	if (i >= 0)
		currpid = dequeue(multiqueue[i]);
	else
		currpid = 0; // no ready processes, so schedule null process


	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;

	//preempt = QUANTUM;		/* Reset time slice for process	*/
	preempt = getTSQuantum();
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
