#include <xinu.h>

SendEntry sendtab[NSQENT];

/*------------------------------------------------------------------------
 *  sendGetitem  -  Remove a process from an arbitrary point in a queue
 *------------------------------------------------------------------------
 */
pid32 sendGetitem(
	  pid32		pid		/* ID of process to remove	*/
	)
{
	pid32	prev, next;

	next = sendtab[pid].qnext;	/* Following node in list	*/
	prev = sendtab[pid].qprev;	/* Previous node in list	*/
	sendtab[prev].qnext = next;
	sendtab[next].qprev = prev;
	return pid;
}

/*------------------------------------------------------------------------
 *  sendGetfirst  -  Remove a process from the front of a queue
 *------------------------------------------------------------------------
 */
pid32 sendGetfirst(
	  qid16		q		/* ID of queue from which to	*/
	)				/* Remove a process (assumed	*/
					/*   valid with no check)	*/
{
	pid32	head;

	if (sisempty(q)) {
		return EMPTY;
	}

	head = squeuehead(q);
	return sendGetitem(sendtab[head].qnext);
}

/*------------------------------------------------------------------------
 *  sendEnqueue  -  Insert a process at the tail of a send queue
 *------------------------------------------------------------------------
 */
 pid32 sendEnqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	int	tail, prev;		/* Tail & previous node indexes	*/

	if (sisbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = squeuetail(q);
	prev = sendtab[tail].qprev;

	sendtab[pid].qnext  = tail;	/* Insert just before tail node	*/
	sendtab[pid].qprev  = prev;
	sendtab[prev].qnext = pid;
	sendtab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  sendDequeue  -  Remove and return the first process on a send list
 *------------------------------------------------------------------------
 */
pid32 sendDequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (sisbadqid(q)) {
		return SYSERR;
	} else if (sisempty(q)) {
		return EMPTY;
	}

	pid = sendGetfirst(q);
	sendtab[pid].qprev = EMPTY;
	sendtab[pid].qnext = EMPTY;
	return pid;
}

/*------------------------------------------------------------------------
 *  sendNewqueue  -  Allocate and initialize a queue in the global queue table
 *------------------------------------------------------------------------
 */
qid16 sendNewqueue(void)
{
	static qid16	nextqid=NPROC;	/* Next list in sendtab to use	*/
	qid16		q;		/* ID of allocated queue 	*/

	q = nextqid;
	if (q > NSQENT) {		/* Check for table overflow	*/
		return SYSERR;
	}

	nextqid += 2;			/* Increment index for next call*/

	/* Initialize head and tail nodes to form an empty queue */

	sendtab[squeuehead(q)].qnext = squeuetail(q);
	sendtab[squeuehead(q)].qprev = EMPTY;
	sendtab[squeuehead(q)].qkey  = MAXKEY;
	sendtab[squeuetail(q)].qnext = EMPTY;
	sendtab[squeuetail(q)].qprev = squeuehead(q);
	sendtab[squeuetail(q)].qkey  = MINKEY;
	return q;
}
