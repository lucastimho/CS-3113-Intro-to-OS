#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>

/* semaphore key */
#define SEMKEY ((key_t)400L)

/* number of semaphores */
#define NUMSEMS 1

/* GLOBAL */
int sem_id; // semaphore id

/* semaphore buffers */
static struct sembuf OP = {0, -1, SEM_UNDO};
static struct sembuf OV = {0, 1, SEM_UNDO};

/* semapore union */
typedef union
{
  int val;
  struct semid_ds *buf;
  ushort *array;
} semunion;

semunion semctl_arg;

/* semaphore function */
int POP()
{
  return semop(sem_id, &OP, 1);
}

/* function for semaphore to release protection */
int VOP()
{
  return semop(sem_id, &OV, 1);
}

/* Define a shared memory */
typedef struct
{
  int val;
} shared_mem;

shared_mem *tot;

/* Each function increments the value of total variable shared among processes by 1 until it reaches a certain threshold */
void p1()
{
  int i = 0;
  while (i < 100000)
  {
    POP();
    tot->val += 1;
    VOP();
    i++;
  }
  printf("process 1: counter =  %d.\n", tot->val);
}
void p2()
{
  int i = 0;
  while (i < 200000)
  {
    POP();
    tot->val += 1;
    VOP();
    i++;
  }
  printf("process 2: counter =  %d.\n", tot->val);
}
void p3()
{
  int i = 0;
  while (i < 300000)
  {
    POP();
    tot->val += 1;
    VOP();
    i++;
  }
  printf("process 3: counter =  %d.\n", tot->val);
}
void p4()
{
  int i = 0;
  while (i < 500000)
  {
    POP();
    tot->val += 1;
    VOP();
    i++;
  }
  printf("process 4: counter =  %d.\n", tot->val);
}

int main()
{

  /* Generate an unique key based on the path and id by using funtion keytok() key is used for identifying shared memory segment */
  char *path = "./";
  int id = 1;
  key_t key = keytok(path, id);

  int shmid, pid1, pid2, pid3, pid4;
  char *shmadd;
  shmadd = (char *)0;

  /* Create and connect to a shared memory segment */
  if ((shmid = shmget(key, sizeof(int), IPC_CREAT | 0666)) < 0)
  {
    perror("shmget");
    exit(1);
  }
  if ((tot = (shared_mem *)shmat(shmid, shmadd, 0)) == (shared_mem *)-1)
  {
    perror("shmat");
    exit(0);
  }

  /* Initialize shared memory to 0 */
  tot->val = 0;

  semunion semctl_arg;
  semctl_arg.val = 1;

  /* Create semaphores */
  sem_id = semget(SEMKEY, NSEMS, IPC_CREAT | 0666);
  if (sem_id < 0)
    printf("Error in creating the semaphore./n");

  /* Initialize semaphore */
  if (semctl(sem_id, 0, SETVAL, semctl_arg) < 0)
    printf("Error detected in SETVAL.\n");

  /* Create 4 pes */
  if ((pid1 = fork()) == 0)
  {
    p1();
    exit(0);
  }
  if ((pid1 != 0) && (pid2 = fork()) == 0)
  {
    p2();
    exit(0);
  }
  if ((pid1 != 0) && (pid2 != 0) && (pid3 = fork()) == 0)
  {
    p3();
    exit(0);
  }
  if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && (pid4 = fork()) == 0)
  {
    p4();
    exit(0);
  }

  /* Parent wait for child pes to finish */
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  waitpid(pid3, NULL, 0);
  waitpid(pid4, NULL, 0);

  printf("\n");

  /* Parent print ID of each child */
  printf("Child with ID %d has just exited.\n", pid1);
  printf("Child with ID %d has just exited.\n", pid2);
  printf("Child with ID %d has just exited.\n", pid3);
  printf("Child with ID %d has just exited.\n", pid4);

  printf("\n");

  /* Deallocate semaphore */
  semctl_arg.val = 0;
  if (semctl(sem_id, 0, IPC_RMID, semctl_arg) < 0)
    printf("Error in removing the semaphore.\n");

  /* Detach the shared memory */
  if (shmdt(tot) == -1)
  {
    perror("shmdt");
    exit(-1);
  }

  /* Remove a shared memory and terminate */
  shmctl(shmid, IPC_RMID, NULL);
  printf("End of Program\n");
  return 0;
}
