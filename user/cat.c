#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[512];

void
cat_start_with_name(int fd, char *name)
{
  int n;
  printf("\n[cat]=='%s'==[cat]\n" , name);
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      fprintf(2, "[cat]: failed to write!\n");
      exit(1);
    }
  }
    if (n < 0) {
      fprintf(2, "[cat]: failed to read!\n");
      exit(1);
    }
  printf("\n[end]=='%s'==[end]\n" , name);
  
}
void
cat(int fd)
{
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      fprintf(2, "cat: write error\n");
      exit(1);
    }
  }
  if(n < 0){
    fprintf(2, "cat: read error\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;
  //char filename[512];
  if(argc <= 1){
    cat(0);
    exit(0);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      exit(1);
    //*filename = *argv[i];
    }
    cat(fd);
    //cat_start_with_name(fd, filename);
    close(fd);
  }
  exit(0);
}
