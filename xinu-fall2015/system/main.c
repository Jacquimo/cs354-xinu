/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define SLEEP_MS 200

extern void lab0app1(void); /* testing process */
extern void lab0app2(void);

extern long host2netl(long);
extern void printsegaddress(void);
extern int stackdepth(void);
extern syscall stacktrace(int);
extern void printloop(char);
extern void printnoloop(char);

extern void myvictim(void);
extern void myattacker(void);
extern void cpuintensive(void);
extern void iointensive(void);

int myvictimglobal = 0;

void test(int rec) {
	if (rec > 0)
		test(rec - 1);
	else
		printf("Stack Depth from \"test(%d)\": %d\n", rec, stackdepth());
}

process	main(void)
{

	// Lab 0
	/*
	resume (create((void *)lab0app1, 65536, 20, "lab01app1", 0, NULL));
	sleepms(SLEEP_MS);

	resume (create((void *)lab0app2, 65536, 20, "lab01app2", 0, NULL));
	sleepms(SLEEP_MS);
	*/
	// -----------------------------------------------------------------------


	// Lab 01 - Part 3
	//int swap = 1000;
	//printf("\nSwaping 0x%08X byte order: 0x%08X\n", swap, host2netl(swap));


	// Lab 01 - Part 3
	//printsegaddress();
	//printf("Stack Depth: %d\n", stackdepth());
	//test(3);


	// Lab 01 - Part 4
	/*
	int p1, p2, p3, p4;
	p1 = create((void*)printloop, 1024, 20, "printloop - A", 1, 'A', NULL);
	p2 = create((void*)printloop, 1024, 20, "printloop - B", 1, 'B', NULL);
	p3 = create((void*)printloop, 1024, 50, "printloop - C", 1, 'C', NULL);
	p4 = create((void*)printloop, 1024, 50, "printloop - D", 1, 'D', NULL);

	putchar('P');
	resume(p1);
	putchar('P');
	resume(p2);
	putchar('P');
	resume(p3);
	putchar('P');
	resume(p4);

	sleepms(4000);
	*/

	// Lab 01 - Part 5
	/*int p1, p2, p3, p4;
	p1 = create((void*)printnoloop, 1024, 50, "printnoloop - A", 1, 'A', NULL);
	p2 = create((void*)printnoloop, 1024, 50, "printnoloop - B", 1, 'B', NULL);
	p3 = create((void*)printnoloop, 1024, 50, "printnoloop - C", 1, 'C', NULL);
	p4 = create((void*)printnoloop, 1024, 50, "printnoloop - D", 1, 'D', NULL);

	resume(p1);
	sleepms(12000);
	resume(p2);
	sleepms(12000);
	resume(p3);
	sleepms(12000);
	resume(p4);
	sleepms(12000);

	printf("\nPrintNoLoop \"%c\" stack depth: %d\n", 'P', stackdepth());
	//sleepms(10000);
	// -----------------------------------------------------------------------
	*/



	// Lab 02 - Part 3: Hijacking a Process through Stack Smashing

	//sleepms(1000);
	int mv = create(myvictim, 4096, 20, "myvictim", 0, NULL);
	int ma = create(myattacker, 4096, 20, "myattacker", 0, NULL);

	resume(mv);
	resume(ma);

	sleepms(3000);

	// Lab 02 - Part 4: Monitoring CPU Usage of Processes
	kprintf("\n\n");

	int p1, p2, p3, p4;
	p1 = create((void*)printloop, 1024, 20, "printloop - A", 1, 'A', NULL);
	p2 = create((void*)printloop, 1024, 20, "printloop - B", 1, 'B', NULL);
	p3 = create((void*)printloop, 1024, 50, "printloop - C", 1, 'C', NULL);
	p4 = create((void*)printloop, 1024, 50, "printloop - D", 1, 'D', NULL);

	putchar('P');
	resume(p1);
	putchar('P');
	resume(p2);
	putchar('P');
	resume(p3);
	putchar('P');
	resume(p4);

	sleepms(2000);

	kprintf("\n\n");

	// Lab 02 - Part 5: Fair Time Share Scheduling

	// Test: All CPU-Intensive
	
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
	

	// -----------------------------------------------------------------------


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

#undef SLEEP_MS
