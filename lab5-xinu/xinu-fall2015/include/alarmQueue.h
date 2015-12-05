// Making different queue (that is the same a normal queue) for alarms

/* Queue structure declarations, constants, and inline functions	*/

/* Default # of queue entries: 1 per process plus 2 for ready list plus	*/
/*			2 for sleep list plus 2 per semaphore		*/
/*#ifndef NQENT
#define NQENT	(NPROC + 4 + NSEM + NSEM)
#endif
*/

#ifndef NAQENT
#define NAQENT (NPROC + 4)
#endif

typedef struct aqentry	{		/* One per process plus two per list	*/
	int32	qkey;		/* Key on which the queue is ordered	*/
	qid16	qnext;		/* Index of next process or tail	*/
	qid16	qprev;		/* Index of previous process or head	*/
} AlarmQueueEntry;

extern AlarmQueueEntry aqueuetab[];

/* Inline queue manipulation functions */

#define	aqueuehead(q)	(q)
#define	aqueuetail(q)	((q) + 1)
#define	afirstid(q)	(aqueuetab[aqueuehead(q)].qnext)
#define	alastid(q)	(aqueuetab[aqueuetail(q)].qprev)
#define	aisempty(q)	(afirstid(q) >= NPROC)
#define	anonempty(q)	(afirstid(q) <  NPROC)
#define	afirstkey(q)	(aqueuetab[afirstid(q)].qkey)
#define	alastkey(q)	(aqueuetab[alastid(q)].qkey)

/* Inline to check queue id assumes interrupts are disabled */

#define	aisbadqid(x)	(((int32)(x) < 0) || (int32)(x) >= NAQENT-1)
