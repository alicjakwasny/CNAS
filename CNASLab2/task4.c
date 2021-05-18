#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//to compile: clang-7 -pthread -o task4 task4.c
//ps -f <pid>

//If the parent finishes execution before child, child is adopted by the main 'init' process that has pid equal to 1

//If the child finishes first, it goes to zombie state and waits untill the parent process will release its resources (e.g. if the child process terminated with error or not)

void child_task(void)
{
  printf("Child process...\n");
  printf("My pid: %d \n", getpid());
  printf("My mum's pid: %d \n", getppid());
}

int main(void) {
  pid_t pid; //creating first process

  pid = fork();

  if (pid == -1)//checking for errors
  {
    perror("Fork: ");
    exit(1);
    //it's important to stop the process from running
  }

  if ( pid ==0 )
  {
    child_task();//create some functions instead of writing everything here
		sleep(1);
		exit(0);
  }

  printf("Parent process...\n");
  printf("My pid: %d \n", getpid());
  printf("My mum's pid: %d \n", getppid());

  sleep(5);

  return 0;
}