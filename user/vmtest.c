#include "kernel/types.h"
#include "user/user.h"



int 
main(int argc, char *argv[])
{
	
	sbrk(1);

	exit(0);
}
