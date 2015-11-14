/* sendQueue.h - firstid, firstkey, isempty, lastkey, nonempty */

/* Sending Queue structure declarations, constants, and inline functions */

#ifndef NSQENT
#define NSQENT (NPROC * 3)
#endif

typedef struct sendEntry	{		/* One per process plus two per list */
	int32	qkey;					/* Key on which the queue is ordered */
	qid16	qnext;					/* Index of next process or tail */
	qid16	qprev;					/* Index of previous process or head */
} SendEntry;

extern SendEntry sendtab[];

/* Inline queue manipulation functions */

#define	squeuehead(q)	(q)
#define	squeuetail(q)	((q) + 1)
#define	sfirstid(q)	(sendtab[squeuehead(q)].qnext)
#define	slastid(q)	(sendtab[squeuetail(q)].qprev)
#define	sisempty(q)	(sfirstid(q) >= NPROC)
#define	snonempty(q)	(sfirstid(q) <  NPROC)
#define	sfirstkey(q)	(sendtab[sfirstid(q)].qkey)
#define	slastkey(q)	(sendtab[slastid(q)].qkey)

/* Inline to check queue id assumes interrupts are disabled */

#define	sisbadqid(x)	(((int32)(x) < 0) || (int32)(x) >= NSQENT-1)
