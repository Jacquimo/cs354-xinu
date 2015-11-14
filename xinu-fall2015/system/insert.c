/* insert.c - insert */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  insert  -  Insert a process into a queue in ascending key order
 *------------------------------------------------------------------------
 */
status	insert(
	  pid32		pid,	/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  uint32	key		/* Key for the inserted process	*/
	)
{
	int16	curr;			/* Runs through items in a queue*/
	int16	prev;			/* Holds previous node index	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	curr = firstid(q);
	
	// search for entry in list when priority is based on proc->prprio
	// priorities are sorted from largest declared priority to smallest
	/*while (queuetab[curr].qkey >= key) {
		curr = queuetab[curr].qnext;
	}*/

	/* Lab 2 - Fair Scheduler
	   Search for entry in list when priority is based on proc->prcpuused.
	   Priorities are sorted from least cpu time used to largest.
	   NOTE: by comparing keys using '<=' instead of '<', the round robin
			 functionality for processes with the same CPU usage will remain.
	*/
	while (queuetab[curr].qkey <= key) {

		curr = queuetab[curr].qnext;
	}

	// Insert process between curr node and previous node

	prev = queuetab[curr].qprev;	// Get index of previous node
	queuetab[pid].qnext = curr;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[prev].qnext = pid;
	queuetab[curr].qprev = pid;
	return OK;
}
