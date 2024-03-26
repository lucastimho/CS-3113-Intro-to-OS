/*Lucas Ho
Ho0055
gpel9*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

/*key number*/
#define SHMKEY ((key_t)1497)

/*shared memory*/
typedef struct
{
  int value;
} shared_mem;
shared_mem *total;

/*Increments total by 100000*/
void process1()
{
  for (int i = 0; i < 100000; i++)
    total->value++;
}
/*Increments total by 200000*/
void process2()
{
  for (int i = 0; i < 200000; i++)
    total->value++;
}
/*Increments total by 300000*/
void process3()
{
  for (int i = 0; i < 300000; i++)
    total->value++;
}
/*Increments total by 500000*/
void process4()
{
  for (int i = 0; i < 500000; i++)
    total->value++;
}

int main()
{
  /*Initialize variables*/
  int shmid;
  pid_t pid1, pid2, pid3, pid4;

  char *shmadd;
  shmadd = (char *)0;

  /*Create and connect to a shared memory segment*/
  if ((shmid = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0)
  {
    perror("shmget");
    exit(1);
  }
  if ((total = (shared_mem *)shmat(shmid, shmadd, 0)) ==
      (shared_mem *)-1)
  {
    perror("shmat");
    exit(0);
  }

  /*Initalize shared memory to 0*/
  total->value = 0;

  /*Fork four child processes pid1, pid2, pid3, pid4*/
  pid1 = fork();

  if (pid1 == -1)
  {
    perror("fork");
    exit(1);
  }
  else if (pid1 == 0)
  {
    process1();
    printf("From Process 1: counter = %d.\n", total->value);
    total->value = 0;
    exit(0);
  }

  pid2 = fork();

  if (pid2 == -1)
  {
    perror("fork");
    exit(1);
  }
  else if (pid2 == 0)
  {
    process2();
    printf("From Process 2: counter = %d.\n", total->value);
    total->value = 0;
    exit(0);
  }

  pid3 = fork();

  if (pid3 == -1)
  {
    perror("fork");
    exit(1);
  }
  else if (pid3 == 0)
  {
    process3();
    printf("From Process 3: counter = %d.\n", total->value);
    total->value = 0;
    exit(0);
  }

  pid4 = fork();

  if (pid4 == -1)
  {
    perror("fork");
    exit(1);
  }
  else if (pid4 == 0)
  {
    process4();
    printf("From Process 4: counter = %d.\n", total->value);
    total->value = 0;
    exit(0);
  }

  /*Wait for child processes to finish*/
  waitpid(pid1, NULL, 0);
  printf("Child with ID: %d has just exited.\n", pid1);
  waitpid(pid2, NULL, 0);
  printf("Child with ID: %d has just exited.\n", pid2);
  waitpid(pid3, NULL, 0);
  printf("Child with ID: %d has just exited.\n", pid3);
  waitpid(pid4, NULL, 0);
  printf("Child with ID: %d has just exited.\n", pid4);

  /*Detach the shared memory segment from the parent process*/
  if (shmdt(total) == -1)
  {
    perror("shmdt");
    exit(1);
  }

  /*Remove the shared memory segment*/
  if (shmctl(shmid, IPC_RMID, NULL) == -1)
  {
    perror("shmctl");
    exit(1);
  }

  printf("End of Simulation.\n");

  return 0;
}