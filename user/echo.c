#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  for(i = 1; i < argc; i++){
    write(1, argv[i], strlen(argv[i]));
    if(i + 1 < argc){
      write(1, " ", 1);
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}

uint starts_with_(char *str, char target) 
{
  if (str[0] == '\"' || str[0] == '\'' || str[0] == target)
      return 1;
  return 0;
}
uint ends_with_(char *str, char target) 
{
  while (*str) {
    str++;
  }

  if (*str == '\"' || *str == '\'' || *str == target)
      return 1;
  return 0;
}