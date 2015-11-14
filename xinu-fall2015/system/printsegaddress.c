#include <xinu.h>
#include <stdio.h>

void printsegaddress() {
	//printf("\n\nText- First 4 Bytes: %08X\n", &(etext));
	
	printf("\n\nText Region\n");
	printf("Start: 0x%08X\tLength: %d bytes\n",
		&text, (uint32)&etext - (uint32)&text);

	printf("\nData Region\n");
	printf("Start: 0x%08X\tLength: %d bytes\n",
		&data, (uint32)&edata - (uint32)&data);

	printf("\nBSS Region\n");
	printf("Start: 0x%08X\tLength: %d bytes\n",
		&bss, (uint32)&ebss - (uint32)&bss);
}
