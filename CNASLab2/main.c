#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//to compile: clang-7 -pthread -o main main.c

//both child & parent should display its & its parents process pid
//man 2 get pid
//man 2 getppid

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
		exit(0);
  }
  //child will never go further than here
  /*else
  {
    printf("Parent process...");
  }*/

  printf("Hello\n");
  printf("Parent process...\n");
  printf("My pid: %d \n", getpid());
  printf("My mum's pid: %d \n", getppid());

  sleep(1);

  return 0;
}