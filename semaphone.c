#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

union semun{
  int val; 
  struct seld_ds* bus;
  unsigned short *array;
  struct seminfo *__buf;
};

int main(int argc, char *argv[] ) {
  int semid;
  int key = ftok("semaphone.c", 22);
  int sc;
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0 ) {
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
    printf("semaphore created: %d\n", semid);
    union semun su;
    su.val = 1;
    sc = semctl(semid, 0, SETVAL, su);
    printf("value set: %d\n", sc);
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    semid = semget(key, 1, 0);
    sc = semctl(semid, 0, GETVAL);
    printf("semaphore value: %d\n", sc);
  }
  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    semid = semget(key, 1, 0);
    sc = semctl(semid, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);
  }
  return 0;
}