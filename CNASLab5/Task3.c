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
	close(pipefd[0]);

	int soutfd = dup2(pipefd[1], 1);

	if(soutfd == -1){
		int error = errno;
		printf("Dup2 with error: %s\n", strerror(error));
	}

	fprintf(stdout, "Hey!");
	
	exit(0);
}

int main() {
	int pipefd[2];
  if (pipe(pipefd) < 0) 
  {
		fprintf(stderr, "Error!\n");
		return -1;
	}

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

			int size = 4;
			char read_message[size];

			int er = read(pipefd[0], read_message, size);

			if(er == -1){
				int error = errno;
				printf("Read with error: %s\n", strerror(error));
			}

			printf("PARENT: %s\n", read_message);
			//printf("END OF WORK\n");
		}
		
		return 0;
}