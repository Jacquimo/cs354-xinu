/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		//upgrade process priority because of blocking call
		upgradeProcPrio();

		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/


	// update message queue and the current process' message flags
	if (!sisempty(prptr->sndq)) {
		// message from dequed process will be handled here
		qid16 procToHandle = sendDequeue(prptr->sndq);
		
		prptr->prmsg = (&proctab[procToHandle])->sndmsg;
		prptr->prhasmsg = TRUE;
		unsleep(procToHandle);
	}

	restore(mask);
	return msg;
}
