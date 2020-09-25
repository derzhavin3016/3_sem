#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

__inline int MyErr( char *str_err ) __attribute__((always_inline));

int MyErr( char *str_err )
{
  perror(str_err);
  return errno;
}

int main( int argc, char *argv[] )
{
  if (argc == 1)
    return 1;

  int N = strtol(argv[1], NULL, 0);
  int isParPrint = 0;

  for (int i = 0; i < N; ++i)
  {
    pid_t pid = fork();

    if (pid == -1)
      return MyErr("process error");

    if (pid == 0)
    {
      printf("child %d %d\n", getpid(), getppid());
      exit(0);
    }
    else if (!isParPrint)
    {
      isParPrint = 1;
      printf("parent %d\n", getpid());
    }
  }




  return 0;
}

