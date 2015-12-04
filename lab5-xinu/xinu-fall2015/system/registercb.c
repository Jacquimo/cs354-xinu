// Edit to Code Starts -----------------
// implemented this function

#include <xinu.h>

syscall registercb(int (*func)(void)) {
	proctab[currpid].cbfun = func;
	return OK;
}
