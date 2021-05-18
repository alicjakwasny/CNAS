#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

//to compile: clang-7 -pthread -o main main.c

void child_task()
{
	sleep(5);
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

	//we want to wait for child to finish
	//pid_t child_pid(int *wstatus);
	int status;
	
	pid_t child_pid = wait (&status); //waiting for any child
	//waiting for the child "p" to finish
	waitpid(p, &status, 0);//wait for specific child, check the status of child process but return 
	//WHONANG | WUNTRACED | WCONTINUED imidiately
	//decode child's exit status
	//did it really exit ?

	/*
	WIFEXITED(wstatus)
		returns true if the child terminated normally, that is, by calling exit(3) or _exit(2), or by returning from main().
		*/

		if (WIFEXITED(status))
		{
			//...
		}
		
		//zmiana w pliku
}