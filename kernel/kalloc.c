// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

// lined-list.
struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  // if the pa is note completely devided by PGSIZE OR pa is in the kernel mem OR the pa is over the RAM PHYSTOP.
  // panic!
  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs. Total page was filled with junk!
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;// let the address pa stor a struct run

  acquire(&kmem.lock);
  // head-push
  r->next = kmem.freelist;
  kmem.freelist = r;// new free mem is set as the header of the linked-list. ...
  release(&kmem.lock);
}

// Allocate one **4096-byte page** of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  // if !r => the mem has gone out.
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 2, PGSIZE); // fill with junk(can be any valid integer. 5, 4, 3, 2, ....)
  return (void*)r;
}

uint64
kfreemem(void)
{

	long freesize;
	struct run *runp;
	runp = kmem.freelist;
	int pagenum = 0;
	while (runp->next && pagenum < 64) 
		pagenum++;
  freesize = pagenum * PGSIZE;
	return freesize;

}