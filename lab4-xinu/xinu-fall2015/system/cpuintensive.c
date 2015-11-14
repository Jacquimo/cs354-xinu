#include <xinu.h>

#define LOOP1 6
#define LOOP2 5000000


void cpuintensive() {
	struct procent* proc = &proctab[currpid];

	int i = 0;
	for (; i < LOOP1; ++i) {
		uint32 k = 53;
		uint32 j = 0;
		for (; j < LOOP2; ++j) {
			k = k * (k + 3);
			k += 663;
			k /= 11;
			k *= 13;
		}
		kprintf("\nPID = %d\tCPU-Intensive \"%s\"\nOuter Loop = %d\n"
			"Priority = %u\nTime Slice Remaining = %u\n",
			currpid, proc->prname, i, proc->prprio, preempt);
	}
	//kprintf("\nPID = %d:\tTotal CPU Time Used = %u\n", currpid, proc->prcpuused);
}

#undef LOOP1
#undef LOOP2

