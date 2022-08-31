
#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"


int 
main(int argc, char* argv[])
{

	

	struct sysinfo info;

	memset((void *)&info, 0, sizeof(struct sysinfo));
	//memset((void *)&info, 0, sizeof(struct sysinfo));
	// we call the function in the user mode
	sysinfo((struct sysinfo*)&info);
	printf("active proc number:%l, ,freemem:%l\n", info.nproc, info.freemem);


	exit(0);
}