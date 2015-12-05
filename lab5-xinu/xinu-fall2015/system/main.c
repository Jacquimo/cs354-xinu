/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

int myalrmhandler() {
	kprintf("This is your alarm going off. Wake up, or you'll be late for "\
		"Spiderman's awesome party.\n");
	return OK;
}

int mycpuhandler() {
	kprintf("PID: %d, stop being a CPU hog!\n", currpid);
	return OK;
}

process	main(void)
{
	if (registercbsig(MYSIGALRM, &myalrmhandler, 2000) != OK) {
		kprintf("alarm handler registration failed\n");
		return 1;
	}


	kprintf("\nHello World!\n");
	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
	kprintf("\n...creating a shell\n");

	recvclr();
	resume(create(shell, 8192, INITPRIO, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}

	return OK;
}
