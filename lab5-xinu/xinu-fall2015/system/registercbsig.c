// TA Note: Implemented this whole file

#include <xinu.h>

syscall registercbsig(uint16 asig, int (*func) (void), uint32 optarg) {
	intmask mask = disable();
	int ret = OK;

	switch(asig) {

		case MYSIGRECV:
			proctab[currpid].cbfun = func;
			break;

		case MYSIGALRM:
			break;

		default:
			ret = SYSERR;
	}

	restore(mask);
	return ret;
}


