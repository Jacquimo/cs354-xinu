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

	// Perform error checking
	mask = disable();
	prptr = &proctab[pid];

	do {
		if (isbadpid(pid) || maxwait < 0 || (prptr->prstate == PR_FREE)) {
			restore(mask);
			return SYSERR;
		}

		// Receiving process already has a message
		if (prptr->prhasmsg == TRUE) {
			// insert into sleep queue if maxwait != 0
			/*if (maxwait > 0) {
				if (insertd(currpid, sleepq, maxwait) == SYSERR) {
					restore(mask);
					return SYSERR;
				}
			}*/

			// if wait maxwait is 0, sleep process for maximum amount of time
			if (insertd(currpid, sleepq, maxwait > 0 ? maxwait : MAXKEY) == SYSERR) {
				restore(mask);
				return SYSERR;
			}		

			upgradeProcPrio();
			(&proctab[currpid])->prstate = PR_SEND;
			resched();
		}

	// put to sleep again if infinitely waiting and timer expires
	} while (maxwait == 0 && prptr->prhasmsg == TRUE);

	// Either there is no other message to handle or the timer expired
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
	}

	// update current process' send values
	prptr = &proctab[currpid];
	prptr->sndmsg = NULL;
	prptr->sndflag = FALSE;

	restore(mask);				// Restore interrupts
	return OK;
}

