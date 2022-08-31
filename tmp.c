// C program to illustrate
// pipe system call in C
// shared by Parent and Child
#include <stdio.h>
#include <unistd.h>
#define MSGSIZE 16
int 
main(void)
{
	int i;
	for (i = 0; i < 9; i++) {
		write(1, &i, sizeof(int));
		printf("\n[%d]=>%c\n", i, *(&i));
	}
	


	return 0;
}
