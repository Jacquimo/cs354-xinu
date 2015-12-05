/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/


	// Edits to code here --------------------------
	// Monitor the amount of CPU usage by a process
	struct procent *prptr = &proctab[currpid];
	(prptr->prcpuused)++;
	
	if (prptr->sighandler == MYSIGXCPU && prptr->cbfun != NULL 
		&& prptr->prcpuused >= prptr->usageLimit) {

		prptr->sighandler = 0;			// reset the signal handler
		(prptr->cbfun)();				// execute callback function
		prptr->cbfun = NULL;
		prptr->usageLimit = 0;
	}
	
	// Handle processes waiting on an alarm by decrementing the front element
	if (!aisempty(alarmq)) {
		if ((--aqueuetab[afirstid(alarmq)].qkey) == 0 ) {
			prptr = &proctab[afirstid(alarmq)];

			// Verify that the callback funct. is still valid
			if (prptr->sighandler == MYSIGALRM && prptr->cbfun != NULL) {
				
				kprintf("Time remaining in alarm = %d\tSignal Handler = %d\tPID = %d\n",
					aqueuetab[afirstid(alarmq)].qkey, prptr->sighandler, afirstid(alarmq));
				
				// execute callback
				(prptr->cbfun)();
				prptr->cbfun = NULL;

				// if the process is asleep, take it out of the sleep queue
				if (prptr->prstate == PR_SLEEP)
					unsleep(afirstid(alarmq));

				// insert into ready list and reschedule
				if (prptr->prstate != PR_READY)
					ready(currpid);
				resched();

				adequeue(afirstid(alarmq));
			}
		}
	}
	// Edits to code here --------------------------


	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}

	/* Handle sleeping processes if any exist */
	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		preempt = QUANTUM;
		resched();
	}
}
