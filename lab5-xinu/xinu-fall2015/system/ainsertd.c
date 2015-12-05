/* insertd.c - insertd */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ainsertd  -  Insert a process in delta list using delay as the key
 *------------------------------------------------------------------------
 */
status	ainsertd(			/* Assumes interrupts disabled	*/
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Delay from "now" (in ms.)	*/
	)
{
	int32	next;			/* Runs through the delta list	*/
	int32	prev;			/* Follows next through the list*/

	if (aisbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	prev = aqueuehead(q);
	next = aqueuetab[aqueuehead(q)].qnext;
	while ((next != aqueuetail(q)) && (aqueuetab[next].qkey <= key)) {
		key -= aqueuetab[next].qkey;
		prev = next;
		next = aqueuetab[next].qnext;
	}

	/* Insert new node between prev and next nodes */

	aqueuetab[pid].qnext = next;
	aqueuetab[pid].qprev = prev;
	aqueuetab[pid].qkey = key;
	aqueuetab[prev].qnext = pid;
	aqueuetab[next].qprev = pid;
	if (next != aqueuetail(q)) {
		aqueuetab[next].qkey -= key;
	}

	return OK;
}
