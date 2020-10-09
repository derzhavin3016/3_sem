#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <getopt.h>

#define BUFFER_SIZE 4096
#define MSG_SIZE 256

#define MAX_ACCESS 0777

__inline int MyErr( char *str_err ) __attribute__((always_inline));

int MyErr( char *str_err )
{
  perror(str_err);
  return errno;
}

typedef struct tagMsg
{
  long type;
  char buf[MSG_SIZE];
} Msg;

int Judge( int que_id, int N )
{
  Msg message;

  printf("Hello, Judge!\n");
  for (int i = 1; i <= N; ++i)
    msgrcv(que_id, &message, MSG_SIZE, N + 1, 0);

  printf("Competition start!\n");

  message.type = N + 2;
  msgsnd(que_id, &message, MSG_SIZE, 0);

  msgrcv(que_id, &message, MSG_SIZE, N, 0);

  printf("End of competition\n");
  for (int i = 1; i <= N; ++i)
  {
    message.type = i;
    msgsnd(que_id, &message, MSG_SIZE, 0);
  }

  return 0;
}

int Runner( int que_id, int number, int N )
{
  Msg message = {number, "Hello"};

  printf("Hello, runner #%d\n", number);
  // inform judge that runner has come
  message.type = N + 1;
  msgsnd(que_id, &message, MSG_SIZE, 0);

  // wait for the palka from previous runner
  msgrcv(que_id, &message, MSG_SIZE, (number == 1) ? N + 2 : (number - 1), MSG_NOERROR);
  // start running
  printf("!runner #%d start\n", number);

  // give palka to next runner
  message.type = number;
  printf("!runner #%d end\n", number);
  msgsnd(que_id, &message, MSG_SIZE, 0);

  // wait for judge
  msgrcv(que_id, &message, MSG_SIZE, number, 0);
  printf("Runner %d go home!\n", number);

  return 0;
}

int main( int argc, char *argv[] )
{
  char buffer[BUFFER_SIZE];

  if (argc < 2)
  {
    printf("Too few arguments.\n");
    return 1;
  }

  setvbuf(stdout, buffer, _IOLBF, BUFFER_SIZE);

  int num = atoi(argv[1]);

  // creator process
  int id = msgget(IPC_PRIVATE, IPC_CREAT | 0700);

  // create judge process
  pid_t pid_jdg = fork();

  if (pid_jdg == 0)
  {
    // judge process
    Judge(id, num);
    return 0;
  }
  for (int i = 1; i <= num; ++i)
  {
    pid_t pid = fork();

    if (pid == -1)
      return MyErr("process error:");

    if (pid == 0)
    {
      // runner
      Runner(id, i, num);
      return 0;
    }
  }


  for (int i = 0; i < num + 1; ++i)
    if (wait(NULL) < 0)
      return MyErr("Process error:");

  msgctl(id, IPC_RMID, NULL);

  return 0;
}
