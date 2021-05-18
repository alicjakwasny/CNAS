/* Write a program that demonstrates the usage of exec() system call (using any version of exec function).

You can try to run 'callendar' program - usually  /usr/bin/cal
*/

//to compile: clang-7 -pthread -o Task1 Task1.c

#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include<errno.h>
#include<string.h>

void child_task()
{
	for ( int i = 1; i<6; i++)
	{
		int ex = execlp("/usr/bin/cal", "cal", "2021");
		if(ex == -1){
				int error = errno;
				printf("Read with error: %s\n", strerror(error));
			}
		sleep(1);
	}
}

int main() {
  pid_t p = fork();
	if (p == -1)
	{
		exit(1);
	}

	if (p==0)
	{
		child_task();
		exit(0);
	}

	int status;
	
	waitpid(p, &status, 0);

		if (WIFEXITED(status))
		{
			printf("END OF WORK\n");
		}
		
		return 0;
}