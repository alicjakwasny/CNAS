#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//to compile: clang-7 -pthread -o main main.c

void child_task(int number)
{
  printf("Child %d process... My pid: %d My mum's pid: %d\n", number, getpid(), getppid());
  //printf("My pid: %d \n", getpid());
  //printf("My mum's pid: %d \n", getppid());
	sleep(1);
}

int main(void) {
  pid_t pid; //creating first process

  if (pid == -1)//checking for errors
  {
    perror("Fork: ");
    exit(1);
  }

  for(int i = 0; i<5; i++){
		pid = fork();
    if (pid == 0){
      child_task(i);
			exit(0);
    }
  }
  
  printf("Hello\n");
  printf("Parent process...\n");
  printf("My pid: %d \n", getpid());
  printf("My mum's pid: %d \n", getppid());

  sleep(1);

  return 0;
}
