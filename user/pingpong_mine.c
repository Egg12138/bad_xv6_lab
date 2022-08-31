
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int
main(int argc, char *argv[])
{
	int pid;
	int p1[2];
	int p2[2];

	pipe(p1);
	pipe(p2);


	pid = fork();

	if (pid < 0) {
		fprintf(2, "fail to fork.\n");
		exit(1);
	} else if (pid == 0){
		// enter the child proc. write to parent. We control them .
		close(p1[1]);
		close(p2[0]);
		printf("%d: received ping\n", getpid());
		close(p1[0]);
		close(p2[1]);
		exit(0);
	} else {
		// back to parent proc
		close(p1[0]);
		close(p2[1]);
		printf("%d: received pong\n", getpid());
		close(p1[1]);
		close(p2[0]);
		exit(0);
	}
	exit(0);

}