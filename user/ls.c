#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
char*
fmtname_withlevel(char *path, int level)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf + level , p, strlen(p));
  for (int i = 0; i < level; i++)
    buf[i] = '\t';
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }


  if (fstat(fd, &st) < 0) {
    fprintf(2, "ls: cannot get file stat:%s\n", path);
    close(fd);
    return;
  }
  // else: open normally.
  printf("| name         | inode | size |\n");
  switch(st.type){
  case T_FILE:
    printf("%s %d   %d   %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf); // end of the path
    *p++ = '/'; // path/, end with `/`
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d    %d    %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}


void
ls_recurse(char *path, int level)
{
  char buf[256];
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(path, 0)) < 0){
    fprintf(2, "ls: failed to open `%s`\n", path);
    return ;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "ls: failed to stat `%s`\n", path);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf("%s %d   %d   %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      printf("ls: path too long\n");
      break;
    }
    char *ptr;
    strcpy(buf, path);
    ptr = buf + strlen(buf);
    if (*ptr != '/')
      *ptr++ = '/';
    if (read(fd, &de, sizeof(de)) == sizeof(de)) {
      memmove(ptr, de.name, sizeof(DIRSIZ)); 
      ls_recurse(ptr, level+1);
    } else {
      return;
    }
    break;
  }
  close(fd);



}
int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  printf("| name         | inode | size |\n");
  for(i=1; i<argc; i++)
    ls_recurse(argv[i], 0);
  exit(0);
}
