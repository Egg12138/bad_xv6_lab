#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char * argv[])
{
	int i;
	char *execargv[MAXARG];
	// we have to check whether the second argument is a number
	if (argc <= 2 || (argv[1][0] < '0') || (argv[1][0] >'9')) {
		fprintf(2, "Need a proc mask\n");
		exit(1);
	}
	printf("--------\n");
	if (trace(atoi(argv[1])) < 0) {
		fprintf(2, "Failed to trace\n");
		exit(1);
	}	

	for (i = 0; i < MAXARG; i++) 
		execargv[i] = argv[i + 2];
	exec(execargv[0], execargv);

	exit(0);
}

	



