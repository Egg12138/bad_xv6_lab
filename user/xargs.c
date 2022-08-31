#include "kernel/types.h"
#include "user/user.h"
#include "user/sh.c"

#define CHILD_PID 0
#define READOFFSET 0
#define WRITEOFFSET 1

extern int getcmd(char *buf, int nbufs);
extern void runcmd(struct cmd* cmd);

int
main(int argc, char *argv[])
{

 
	char buf[100];
	int p[2];
	int pid;

	if (pipe(p) < 0) panic("PIPE ERROR\n");
	if (strcmp(argv[1], "cd") && argc == 3){
		*(argv[1] + strlen(argv[1]) - 1) = 0;
		if (chdir(argv[2]) < 0)
			fprintf(2, "cd: cannot change direct to %s\n", argv[2]);
			exit(0);
	}

	if ((pid = fork()) == 0) {
		runcmd(parsecmd);
	}
	if ((pid = fork()) == 0) {

			
		close(0);
		dup(p[READOFFSET]);
		read(p[READOFFSET], buf, 100);
		close(p[READOFFSET]);
		close(p[WRITEOFFSET]);
		
	}
	
	




	exit(0);


}