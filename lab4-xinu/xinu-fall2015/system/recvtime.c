/* recvtime.c - recvtime */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  recvtime  -  Wait specified time to receive a message and return
 *------------------------------------------------------------------------
 */
umsg32	recvtime(
	  int32		maxwait		/* Ticks to wait before timeout */
        )
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent	*prptr;		/* Tbl entry of current process	*/
	umsg32	msg;			/* Message to return		*/

	if (maxwait < 0) {
		return SYSERR;
	}
	mask = disable();

	/* Schedule wakeup and place process in timed-receive state */

	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {	/* If message waiting, no delay	*/
		if (insertd(currpid,sleepq,maxwait) == SYSERR) {
			restore(mask);
			return SYSERR;
		}

		//upgrade process priority because of blocking call
		upgradeProcPrio();
		prptr->prstate = PR_RECTIM;
		resched();
	}

	/* Either message arrived or timer expired */

	if (prptr->prhasmsg) {
		msg = prptr->prmsg;	/* Retrieve message		*/
		prptr->prhasmsg = FALSE;/* Reset message indicator	*/
	} else {
		msg = TIMEOUT;
	}

	// update message queue and the current process' message flags
	if (!sisempty(prptr->sndq)) {
		// message from dequed process will be handled here
		pid32 procToHandle = sendDequeue(prptr->sndq);

		prptr->prmsg = (&proctab[procToHandle])->sndmsg;
		prptr->prhasmsg = TRUE;

		// receiving process updates sending process to ensure queue can't
		// be potentially jumped by infinite wait senders
		prptr = &proctab[procToHandle];
		prptr->sndmsg = NULL;
		prptr->sndflag = FALSE;
		unsleep(procToHandle);
	}
	
	restore(mask);
	return msg;
}
