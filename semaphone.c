#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>

//char *story weird string address


union semun{
  int val; 
  struct seld_ds* bus;
  unsigned short *array;
  struct seminfo *__buf;
};


int main(int argc, char *argv[] ) {
  int semid;
  int key = ftok("semaphone.c", 22);
  int sc,sd,fd;

  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0 ) {
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
    printf("semaphore created: %d\n", semid);
    union semun su;
    su.val = 1;
    sc = semctl(semid, 0, SETVAL, su);
    printf("value set: %d\n", sc);
    
    //file writing
    fd = open("story",O_CREAT | O_TRUNC, 0644);
    close(fd);
    //Shared Memory
    sd = shmget(key,256 , IPC_CREAT | IPC_EXCL);    
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    semid = semget(key, 1, 0);
    sc = semctl(semid, 0, GETVAL);
    printf("semaphore value: %d\n", sc);
    //read story
    fd = open("story", O_RDONLY);
    int fsize = lseek(fd, 0, SEEK_END);
    lseek(fd,-1 * fsize, SEEK_CUR);
    char buffer[fsize];
    read(fd, buffer, fsize);   
    
    //fd = open("story",O_RDONLY, 0644);
    //char *story;
    //read(fd,story,100);
    close(fd);
    printf("Story:\n %s\n", buffer);
  }
  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    //Printing Story
    fd = open("story", O_RDONLY);
    int fsize = lseek(fd, 0, SEEK_END);
    lseek(fd,-1 * fsize, SEEK_CUR);
    char buffer[fsize];
    read(fd, buffer, fsize);

    //fd = open("story",O_RDONLY, 0644);
    //char *story;
    //read(fd,story,100);
    close(fd);
    printf("Story:\n %s\n", buffer);
    //Removing Semaphore
    semid = semget(key, 1, 0);
    sc = semctl(semid, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);
    //Removing Shared Memory
    sd = shmget(key,256,0644);    
    shmctl(sd,IPC_RMID,0);
  }
  return 0;
}
