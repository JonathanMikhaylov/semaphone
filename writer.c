#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main() {

  int id = ftok("semaphone.c", 22);
  int semid = semget(id, 1, 0);
  int sd = shmget(id, 256, 0644);
  int fd = open("story",  O_APPEND | O_RDWR, 0644);
  
  
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
    
  semop(semid, &sb, 1);

  //stuff to do now
  //getting the last line of story
  int *size = shmat(sd,0,0);
  char lineb4[*size+1];
  lseek(fd, -(*size), SEEK_END);
  read(fd, lineb4, *size);
  lineb4[*size] = 0; //NULL terminating

  //printing
  printf("Previously: %s\n", lineb4);

  //next line to get
  char newline[256];
  printf("Next Line of Story:");
  //getting line from terminal
  fgets(newline,sizeof(newline),stdin);
  write(fd, newline, strlen(newline));
  close(fd);

  //up the semaphore for later access;
  sb.sem_op = 1;
  semop(semid, &sb, 1);

  return 0;
}
