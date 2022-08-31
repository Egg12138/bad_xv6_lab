#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXNUM 41
#define READOFFSET 0
#define WRITEOFFSET 1
#define CHILD_PID 0
int
isrest(int filter, int tobe_checked)
{
	if ( tobe_checked % filter == 0)
		return 0;

	return 1;

}  

void filter(int p_l[])
{
	int pid;
	int n;
	int p_r[2];
	pipe(p_r); //blocked at first
	close(p_l[WRITEOFFSET]);
//	dup(p_l[WRITEOFFSET]);会让缩进不正常

	/* read from left pipe's input */
	if ((read(p_l[READOFFSET], &n, sizeof(int))) <= 0 ) {
		fprintf(1, "Over.\n");
		exit(0);
	} // else read success!
	if ((pid = fork()) == 0) {
		filter(p_r);
	} else { 
		// child proc(regard self as parent proc)
		close(p_r[READOFFSET]);
		int prime = n; // n is keeping updating.
		printf("[39]prime : %d, p_l = {%d, %d}, p_r = {%d, %d}\n", prime, p_l[0], p_l[1], p_r[0], p_r[1]);
		// read a new num(as var: n) and then trap into the loop.  if 
		while ( read(p_l[READOFFSET], &n, sizeof(int)) != 0) {
			if ( isrest(prime, n)) {  // if rest, n will be passed to the right pipe
				write(p_r[WRITEOFFSET], &n, sizeof(int));
			} else {
				printf("[45]n : %d, p_l = {%d, %d}, p_r = {%d, %d}\n", n, p_l[0], p_l[1],  p_r[0], p_r[1]);
			}
		}
		close(p_r[WRITEOFFSET]);
		wait((int *) CHILD_PID);
		exit(0);
	}



}

int 
main(void)
{
	
	int pid;
	int p[2];
	pipe(p);

	if ((pid = fork()) == 0 ) {
		// the child process. read num from the left.
		filter(p);
	} else {
		// the parent process. 
		close(p[READOFFSET]);
		for (int i = 2; i < MAXNUM + 1; i++) {
			write(p[WRITEOFFSET], &i, sizeof(int));
		}
		close(p[WRITEOFFSET]);
		wait((int *) CHILD_PID);

	}


	exit(0);
}