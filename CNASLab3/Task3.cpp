/*
The main program allocates a 1 000 000-element integer array on the heap (free-store)
Then it fills it with random values from the range 0....100

After preparing the array it creates 10 child processes. The task of each of the child processes is to determine the average value of the array fragment (rounded to the nearest integer):
descendant 0 - from index 0 to 99 999
descendant 1 - from index 100 000 to 199 999
descendant 2 - from index 200 000 to 299 999
...

The parent process waits for the descendant processes to complete their calculations and then determines the average value of the array elements based on the obtained partial averages.
*/

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

void child_task(int num, int* pointer, int size) //pointer points to first element to be counted
{
	int sum = 0, avg = 0;
	for (int i = 0; i<size; i++)
	{
		sum += *(pointer+i);
	}
	avg = sum/size;
	printf("Child %d with pid %d average: %d\n", num, getpid(), avg);
	exit(avg);
}

int main() {
	srand( time( NULL ) );
	int ar_size = 1000000;
	int *ar = (int*) malloc(ar_size*sizeof(int)); //rzutowanie na int, bo malloc zwraca void*
	if(ar)
	{
		for (int i = 0; i<ar_size; i++)
		{
			ar[i] = rand() % 101;
		}
	}
	
	pid_t pid;

	int child_ar = 100000; //size of child's part of array
	int children = 10; //how many children processes
	if (pid == -1)
	{
		exit(1);
	}

	for(int i = 0; i<children; i++)
	{
		pid = fork();
  	if (pid == 0){
    	child_task(i, (ar+i*child_ar), child_ar);
			exit(0);
  	}
	}

	int status, sum=0, average;
	//float average;
	for(int i = 0; i<children; i++)
	{
		waitpid(-1, &status, 0);

		if (WIFEXITED(status))
		{
			printf("END OF WORK for a child\n");
			sum += WEXITSTATUS(status);
		}
	}

	average = sum/children;
	//printf("End average: %f\n", average);
	printf("End average: %d\n", average);
	
	free(ar);
	return 0;
}

