#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

void child_task()
{
	for ( int i = 1; i<6; i++)
	{
		printf("%d\n", i);
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