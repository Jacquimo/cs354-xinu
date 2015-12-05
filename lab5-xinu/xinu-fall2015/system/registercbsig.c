// TA Note: Implemented this whole file

#include <xinu.h>

syscall registercbsig(uint16 asig, int (*func) (void), uint32 optarg) {
	intmask mask = disable();

	if (func == NULL) {
		restore(mask);
		return SYSERR;
	}

	int ret = OK;
	struct procent *prptr = &proctab[currpid];

	prptr->cbfun = func;
	switch(asig) {

		// I was told by a TA that there could only be 1 callback function per process
		case MYSIGRECV:
			prptr->sighandler = MYSIGRECV;
			break;

		case MYSIGALRM:
			prptr->sighandler = MYSIGALRM;
			ainsertd(currpid, alarmq, optarg);
			break;

		case MYSIGXCPU:
			prptr->sighandler = MYSIGXCPU;
			break;

		default:
			prptr->cbfun = NULL;
			prptr->sighandler = 0;
			ret = SYSERR;
	}

	restore(mask);
	return ret;
}


