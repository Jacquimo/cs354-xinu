/* sendbt.c - send with (potentially) timed blocking */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendbt - Pass a message to a process and block for a period of time
 *		   - Implementation and structure are based on send.c & recvtime.c
 *------------------------------------------------------------------------
 */

syscall	sendbt(
		pid32	pid,			// ID of recipient process
		umsg32	msg,			// Contents of message
 		int32	maxwait			// ms to block to send message
	)
{
	intmask	mask;				// Saved interrupt mask
	struct	procent *prptr;		// Ptr to process' table entry

	prptr = &proctab[currpid];
	prptr->sndmsg = msg;
	prptr->sndflag = TRUE;

	// Perform error checking
	mask = disable();
	prptr = &proctab[pid];
	if (isbadpid(pid) || maxwait < 0 || (prptr->prstate == PR_FREE)) {
		restore(mask);
		return SYSERR;
	}

	// there is no message to be queued behind, so leave the message
	if (prptr->prhasmsg == FALSE) {
		prptr->prmsg = msg;		// Deliver message
		prptr->prhasmsg = TRUE;	// Indicate message waiting

		// If recipient waiting or in timed-wait, make it ready
		if (prptr->prstate == PR_RECV) {
			ready(pid);
		} else if (prptr->prstate == PR_RECTIM) {
			unsleep(pid);
			ready(pid);
		}
	} else {
		do {
			// need repeated error checking for infinite wait senders
			if (isbadpid(pid) || maxwait < 0 || (prptr->prstate == PR_FREE)) {
				restore(mask);
				return SYSERR;
			}

			// if wait maxwait is 0, sleep process for maximum amount of time
			if (insertd(currpid, sleepq, maxwait > 0 ? maxwait : MAXKEY) == SYSERR) {
				restore(mask);
				return SYSERR;
			}		

			sendEnqueue(currpid, prptr->sndq);
			upgradeProcPrio();
			(&proctab[currpid])->prstate = PR_SEND;
			resched();

		// put to sleep again if infinitely waiting and timer expires
		} while (maxwait == 0 && (&proctab[currpid])->sndflag == TRUE);
	}

	// Either the message has been received or the timer has expired
	// the sending flags of this process are updated by the receiving process

	// check if process was removed from sleep because maxwait was exceeded
	prptr = &proctab[currpid];
	if (prptr->sndflag == TRUE) {
		sendGetitem(currpid);
	}
	prptr->sndflag = FALSE;
	prptr->sndmsg = NULL;

	restore(mask);
	return OK;
}

