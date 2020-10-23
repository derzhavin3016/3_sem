#include "utils.h"

int sem_id = 0;
int shm_id = 0;
char *shm_buf = NULL;

int PrintFile( FILE *fin )
{
  while (1)
  {
    char buffer[BUFFER_SIZE];

    if (fgets(buffer, BUFFER_SIZE, fin) == NULL)
      break;

    // wait for free shared memory
    P(FREE, 1);
    // writing to shared memory
    strcpy(shm_buf, buffer);
    //printf("%s", buffer);
    V(FREE, 1);
    P(NFREE, 1);
  }

  return 1;
}


int main( int argc, char * argv[] )
{
  key_t key = ftok(SERV_NAME, SERV_ID);
  shm_id = shmget(key, BUFFER_SIZE, MAX_ACCESS);
  if (shm_id < 0)
    return MyErr("client:");

  shm_buf = shmat(shm_id, NULL, 0);
  sem_id = semget(key, 2, MAX_ACCESS);
  if (sem_id < 0)
    return MyErr("client:");

  if (argc == 1 || argv[1][0] == '-')
  {
    if (!PrintFile(stdin))
      return 1;
    return 0;
  }

  for (int i = 1; i < argc; ++i)
  {
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0)
    {
      MyErr(argv[i]);
      continue;
    }
    FILE *fin = fdopen(fd, "rb");
    if (fin == NULL)
    {
      MyErr(argv[i]);
      continue;
    }

    if (!PrintFile(fin))
      return 1;

    if (fclose(fin) < 0)
      return MyErr(argv[i]);
  }

  return 0;
}