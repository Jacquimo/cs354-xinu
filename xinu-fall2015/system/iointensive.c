#include <xinu.h>

#define LOOP1 5
#define LOOP2 100

void iointensive() {
	struct procent* proc = &proctab[currpid];

	int i = 0;
	for (; i < LOOP1; ++i) {
		uint32 j = 0;
		for (; j < LOOP2; ++j) {
			sleepms(2);
		}
		kprintf("\nPID = %d\tI/O-Intensive \"%s\"\nOuter Loop = %d\nPriority = 1/%u\n"
				"Time Slice Remaining = %u\n",
			currpid, proc->prname, i, proc->prcpuused, preempt);
	}
	kprintf("\nPID = %d:\tTotal CPU Time Used = %u\n", currpid, proc->prcpuused);
}

#undef LOOP1
#undef LOOP2
