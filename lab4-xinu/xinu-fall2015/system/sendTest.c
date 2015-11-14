#include <xinu.h>

void sendTest(pid32 pid, umsg32 message, int32 wait) {
	sendbt(pid, message, wait);
	kprintf("\nsendTest message ended\n");
}

