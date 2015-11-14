/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

extern void cpuintensive();
extern void iointensive();
extern void sendTest(pid32, umsg32, int32);

process	main(void)
{
	kprintf("\nHello World!\n");
	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
	kprintf("\n...creating a shell\n");


	// Test: All CPU-Intensive
	/*
	kprintf("\n------- CPU-Intensive Muthafuckas -------\n");

	int cpu1 = create(cpuintensive, 4096, 1, "cpu1", 0, NULL);
	int cpu2 = create(cpuintensive, 4096, 1, "cpu2", 0, NULL);
	int cpu3 = create(cpuintensive, 4096, 1, "cpu3", 0, NULL);
	int cpu4 = create(cpuintensive, 4096, 1, "cpu4", 0, NULL);

	resume(cpu1);
	resume(cpu2);
	resume(cpu3);
	resume(cpu4);

	sleepms(4000);
	

	// Test: All I/O-Intensive
	kprintf("\n------- I/O-Intensive Muthafuckas -------\n");

	int io1 = create(iointensive, 4096, 1, "io1", 0, NULL);
	int io2 = create(iointensive, 4096, 1, "io2", 0, NULL);
	int io3 = create(iointensive, 4096, 1, "io3", 0, NULL);
	int io4 = create(iointensive, 4096, 1, "io4", 0, NULL);

	resume(io1);
	resume(io2);
	resume(io3);
	resume(io4);

	sleepms(4000);


	// Test: Half CPU Bound and Half I/O Bound

	kprintf("\n------- Half & Half Muthafuckas -------\n");

	int cpuA = create(cpuintensive, 4096, 1, "cpuA", 0, NULL);
	int cpuB = create(cpuintensive, 4096, 1, "cpuB", 0, NULL);
	int ioA = create(iointensive, 4096, 1, "ioA", 0, NULL);
	int ioB = create(iointensive, 4096, 1, "ioB", 0, NULL);

	resume(cpuA);
	resume(ioA);
	resume(cpuB);
	resume(ioB);

	sleepms(6000);
	*/


	int first = create(sendTest, 4096, 1, "first", 3, currpid, "first message", 0);
	int sec = create(sendTest, 4096, 1, "sec", 3, currpid, "second message", 0);
	int third = create(sendTest, 4096, 1, "third", 3, currpid, "third message", 0);

	resume(first);
	resume(sec);
	resume(third);

	sleepms(1000);

	kprintf("first: \"%s\"\n", receive());
	kprintf("sec: \"%s\"\n", receive());
	kprintf("third: \"%s\"\n", receive());

	sleepms(1000);

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
