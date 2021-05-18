/*As you remember - fork() creates an almost identical copy of the parent process. Interestingly, this copy "inherits" parent's process open file descriptors

The child inherits copies of the parent's set of open file descriptors. Each file descriptor in the child refers to the same open file description (see open(2)) as the corresponding file descriptor in the parent. This means that the two file descriptors share open file status flags, file offset, and signal-driven I/O attributes (see the description of F_SETOWN and F_SETSIG in fcntl(2)).

This means that if you use pipe before fork, you get a pipeline that you can write to in one process and read in another

Please modify your program from Task 2.1 so that the parent writes to the pipe and the child reads and displays received data
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

//to compile: clang-7 -pthread -o Task2 Task2.c

void child_task(int size, int pipefd[2]) //child reads
{
	char read_message[size];
	read(pipefd[0], read_message, size);
  printf("%s\n", read_message);
	exit(0);
}

int main() 
{
	int pipefd[2];
	pid_t p = fork();
  int size = 5;
	char* message = "Hello \n";
	if (p == -1)
	{
		exit(1);
	}

	if (p==0)
	{
		child_task(size, pipefd);
		exit(0);
	}

  //parent writes to file
  write(pipefd[1], message, size);
	return 0;
}