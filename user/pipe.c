#include "kernel/syscall.h"
#include "kernel/types.h"
#include "user/user.h"

#define LEN 2
#define STDOUT 1

int 
main()
{
	int fds[LEN];
	char buf[1024];
	int numread;

	pipe(fds);
	write(fds[0], "$Pip f[0]\n$", 10);
	numread = read(fds[0], buf, sizeof(buf));
	printf("fd[0]=[%d]", fds[0]);
	write(STDOUT, buf, numread);
	
exit(0);
}
