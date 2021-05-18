/*Create a pipe in your main process.

Create a child

Use dup2() system call inside child to override stdout with write end of pipe

Use exec inside child

Your parent should now be able to "intercept" executed program's output on its pipe's read-endpoint.	*/

//to compile: clang-7 -pthread -o Task4 Task4.c

/* Create a pipe in your main process.
Create a child

Use dup2() system call inside child to override stdout with write end of pipe

Your parent should now be able to read anything you "print" inside child from pipe's read-endpoint.
*/

//to compile: clang-7 -pthread -o Task3 Task3.c

#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include<errno.h>
#include<string.h>

void child_task(int pipefd[2])
{
	dup2(pipefd[1], STDOUT_FILENO);

	execl("/usr/bin/cal", "cal", NULL);
  
	close(pipefd[1]);
	
  exit(0);
}

int main() {
	int pipefd[2];
	pipe(pipefd);

  pid_t p = fork();
	if (p == -1)
	{
		exit(1);
	}

	if (p==0)
	{
		child_task(pipefd);
		exit(0);
	}

	int status;
	
	waitpid(p, &status, 0);

		if (WIFEXITED(status))
		{
			close(pipefd[1]);

			int size = 1000;
			char read_message[size];

			int er = read(pipefd[0], read_message, size);

			printf("Parent: %s\n", read_message);
			//printf("END OF WORK\n");
		}
		
		return 0;
}