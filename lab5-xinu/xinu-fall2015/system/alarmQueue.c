/* queue.c - enqueue, dequeue */

#include <xinu.h>

AlarmQueueEntry aqueuetab[NAQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  aenqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	aenqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	int	tail, prev;		/* Tail & previous node indexes	*/

	if (aisbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = aqueuetail(q);
	prev = aqueuetab[tail].qprev;

	aqueuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	aqueuetab[pid].qprev  = prev;
	aqueuetab[prev].qnext = pid;
	aqueuetab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  agetitem  -  Remove a process from an arbitrary point in a queue
 *------------------------------------------------------------------------
 */
pid32	agetitem(
	  pid32		pid		/* ID of process to remove	*/
	)
{
	pid32	prev, next;

	next = aqueuetab[pid].qnext;	/* Following node in list	*/
	prev = aqueuetab[pid].qprev;	/* Previous node in list	*/
	aqueuetab[prev].qnext = next;
	aqueuetab[next].qprev = prev;
	return pid;
}

/*------------------------------------------------------------------------
 *  agetfirst  -  Remove a process from the front of a queue
 *------------------------------------------------------------------------
 */
pid32	agetfirst(
	  qid16		q		/* ID of queue from which to	*/
	)				/* Remove a process (assumed	*/
					/*   valid with no check)	*/
{
	pid32	head;

	if (aisempty(q)) {
		return EMPTY;
	}

	head = aqueuehead(q);
	return agetitem(aqueuetab[head].qnext);
}

/*------------------------------------------------------------------------
 *  adequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	adequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (aisbadqid(q)) {
		return SYSERR;
	} else if (aisempty(q)) {
		return EMPTY;
	}

	pid = agetfirst(q);
	aqueuetab[pid].qprev = EMPTY;
	aqueuetab[pid].qnext = EMPTY;
	return pid;
}

/*------------------------------------------------------------------------
 *  agetlast  -  Remove a process from end of queue
 *------------------------------------------------------------------------
 */
pid32	agetlast(
	  qid16		q		/* ID of queue from which to	*/
	)				/* Remove a process (assumed	*/
					/*   valid with no check)	*/
{
	pid32 tail;

	if (aisempty(q)) {
		return EMPTY;
	}

	tail = aqueuetail(q);
	return agetitem(aqueuetab[tail].qprev);
}

/*------------------------------------------------------------------------
 *  anewqueue  -  Allocate and initialize a queue in the global queue table
 *------------------------------------------------------------------------
 */
qid16	anewqueue(void)
{
	static qid16	nextqid=NPROC;	/* Next list in aqueuetab to use	*/
	qid16		q;		/* ID of allocated queue 	*/

	q = nextqid;
	if (q > NAQENT) {		/* Check for table overflow	*/
		return SYSERR;
	}

	nextqid += 2;			/* Increment index for next call*/

	/* Initialize head and tail nodes to form an empty queue */

	aqueuetab[aqueuehead(q)].qnext = aqueuetail(q);
	aqueuetab[aqueuehead(q)].qprev = EMPTY;
	aqueuetab[aqueuehead(q)].qkey  = MAXKEY;
	aqueuetab[aqueuetail(q)].qnext = EMPTY;
	aqueuetab[aqueuetail(q)].qprev = aqueuehead(q);
	aqueuetab[aqueuetail(q)].qkey  = MINKEY;
	return q;
}
