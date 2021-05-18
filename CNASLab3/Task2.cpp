#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

void child_task(int a, int b)
{
	int sum = a+b;
	printf("Child sum: %d\n", sum);
	exit(sum);
}

int main() {
	srand( time( NULL ) );

	int a = rand() % 10;
	printf("a = %d\n", a);
	int b = rand() % 30 + 20;
	printf("b = %d\n", b);
  pid_t p = fork();
	if (p == -1)
	{
		exit(1);
	}

	if (p==0)
	{
		child_task(a, b);
		exit(0);
	}

	int status;
	
	waitpid(p, &status, 0);

		if (WIFEXITED(status))
		{
			printf("END OF WORK\n");
			printf("Sum: %d\n", WEXITSTATUS(status)); //jak dacie tutaj te WEXITSTATUS(status) w tym int zamiast status to dodaje dobrze i nie trzeba tego na gorze
		}
		
		return 0;
}