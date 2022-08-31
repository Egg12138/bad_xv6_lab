#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int 
main(int argc, char *argv[])
{
	if (argc == 1) {
		fprintf(2, "Error: Sleep second required!\n");
		exit(0);
	} 
	if (argc >= 2) {
		if (!('0' <= *argv[1] && *argv[1] <= '9')) {
			fprintf(2, "Error: Sleep second needs to be int!\n");
			exit(0);
		}
		sleep(atoi(argv[1]));
		exit(0);
	} 
	exit(0);
}