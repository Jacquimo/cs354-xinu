
#define NUM_PRIO_LVLS 60

#define DEFAULT_PRIO 29

struct ts_disptb {
	int ts_quantum;       // new time slice
	int ts_tqexp;         // new priority: CPU-bound (time quantum expired)
	int ts_slpret;        // new priority: I/O-bound (sleep return)
};

extern struct ts_disptb tsdtab[NUM_PRIO_LVLS];

#define upgradeProcPrio() (proctab[currpid].prprio = \
							tsdtab[proctab[currpid].prprio].ts_slpret)

#define downgradeProcPrio() (proctab[currpid].prprio = \
							tsdtab[proctab[currpid].prprio].ts_tqexp)

#define getTSQuantum() (tsdtab[proctab[currpid].prprio].ts_quantum)
