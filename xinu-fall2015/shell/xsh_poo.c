/* xsh_poo.c - xsh_poo */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*-------------------------------------------------------------------------------------
 * xsh_poo  -  Shell command to show how XINU is a shit operating system with ASCII art
 * url: http://www.ascii-art.de/ascii/t/toilets.txt
 *-------------------------------------------------------------------------------------
 */
shellcmd xsh_poo(int nargs, char *args[]) {
	
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays a ASCII art graphic that details the workings of the " 
				"XINU Operating System\n");
		printf("Options:\n");
		printf("\tThere are no options or flags for this command\n");
		return 0;
	}

printf( "\n"
		"               Depiction of the XINU Operating System\n\n"
		"              .----------/ |<=== floppy disk\n"
		"             /           | |\n"
		"            /           /| |          _________\n"
		"           /           / | |         | .-----. |\n"
		"          /___________/ /| |         |=|     |-|\n"
		"         [____________]/ | |         |~|_____|~|\n"
		"         |       ___  |  | |         '-|     |-'\n"
		"         |      /  _) |  | |           |.....|\n"
		"function ======>|.'   |  | |           |     |<=== application\n"
		"  key    |            |  | |    input  |.....|       software\n"
		"         |            |  | |            `--._|\n"
		"  main =>|            |  | |      |\n"
		" storage |            |  | ;______|_________________\n"
		"         |            |  |.' ____\\|/_______________ `.\n"
		"         |            | /|  (______________________)  )<== user\n"
		"         |____________|/ \\___________________________/  interface\n"
		"         '--||----: `'''''.__                      |\n"
		"            || jgs `\"\"\";\"\"\"-.'-._ <== normal flow  |    central\n"
		"            ||         |     `-. `'._of operation /<== processing\n"
		"    ||      ||         |        `\\   '-.         /       unit\n"
		"  surge     ().-.      |         |      :      /`\n"
		"control ==>(_((X))     |      .-.       : <======= output\n"
		" device       '-'      \\     |   \\      ;      |________\n"
		"    ||                  `\\  \\|/   '-..-'       / /_\\   /|\n"
		"    ||                   /`-.____             |       / /\n"
		"    ||                  /  _    /_____________|_     / /_\n"
		"    ||    peripherals ==>/_\\___________________/_\\__/ /~ )__\n"
		"    ||      (hardware) |____________________________|/  ~   )\n"
		"    ||                                     (__~  ~     ~(~~`\n"
		"    ||    overflow (input/output error) ===> (_~_  ~  ~_ `)\n"
		"  .-''-.                                         `--~-' '`\n"
		" /______\\                              _________\n"
		"  [____] <=== de-bugging tool       _|`---------`|\n"
		"                                   (C|           |\n"
		"                        back-up ===> \\           /\n"
		" |\\\\\\ ///|                            `=========`\n"
		" | \\\\V// |\n"
		" |  |~|  |\n"
		" |  |=|  | <=== supplemental data\n"
		" |  | |  |\n"
		" |  | |  |                          (()____\n"
		"  \\ |=| /              mouse ===>  ('      `\\_______,\n"
		"   \\|_|/                            `,,---,,'\n"

   );
	return 0;
}
