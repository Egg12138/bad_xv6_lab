#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"



uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64
sys_trace(void)
{
	/* reg:&trmask will stored the value in the [aMask] register.
	* trmask = n => n -> an. 0 -> a0, 1 -> a1.
	* because the width is 32-bits, the n is in the range:[0, 5].->[2^0, 2^5]
	printf(1, "")
	*/
	int trmask; // 0 <= trmask <= 5.

  // fetch the content of reg_a[0] to the address of trmask.
	if (argint(0, &trmask) < 0)
		return -1;
	myproc()->syscalltrmask = trmask;
	return trmask;
}


// which return the free mem & num of used proc, (using bit calculation ) ,from the kernel to the user!
uint64
sys_sysinfo(void)
{

  uint64 sysinfo_p;
	struct sysinfo info;

  // 通过argaddr 使sysinfo_p指向trampframe->a0
  if (argaddr(0, &sysinfo_p) < 0)
    return -1;
  // 所以当前进程的a0的值已经被sysinfo_p指向了。可以进行修改。
	info.nproc = nproc();
	//info.nproc = 1;
  //info.freemem = 1;
	info.freemem = kfreemem();
  // 现在这些结构体信息是在物理地址的，我们要将其copyout到虚拟地址中。
  if (copyout(myproc()->pagetable, sysinfo_p, (char *)&info, sizeof(info)) < 0)
    panic("copyout error!\n");
  return 0;
	
}