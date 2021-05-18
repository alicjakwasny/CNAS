/*
The main program allocates a 1 000 000-element array of floating point numbers on the heap.
Then it fills it with random values from the range -1....1

After preparing the array it creates 10 child processes. The task of each of the child processes is to determine the sum of the elements of the array fragment:
descendant 0 - from index 0 to 99 999
descendant 1 - from index 100 000 to 199 999
descendant 2 - from index 200 000 to 299 999
...
After determining the sum the child processes write it to the file named "sum<PID>.txt" and finish their work.

The parent process waits for the completion of calculations by the child processes and then determines the average value of the array elements based on the contents of files with partial results.
*/

//to compile: clang-7 -pthread -o Task4 Task4.c

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <fcntl.h>
#include <cstdio>
#include<errno.h>
#include<string.h>

float RandomFloat(float min, float max) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

void child_task(int num, float* pointer, int size) //pointer points to first element to be counted
{
	float sum = 0, avg = 0;

	for (int i = 0; i<size; i++)
	{
		sum += *(pointer+i);
		//printf("%f", sum);
	}

	//printf("Child %d sum: %f\n", num, sum);

	int file;
	char buffer[25]; // make sure it's big enough
	snprintf(buffer, sizeof(buffer), "sum%d.txt", getpid());
	file = open(buffer, O_WRONLY|O_CREAT, 0666);

	if(file == 0)
  {
    printf("Unable to create or open the new file\n");
    exit(-1);
  }

	ssize_t nwritten;
	char char_sum[sizeof(float)];
	sprintf(char_sum, "%f", sum);
	nwritten = write(file, char_sum, sizeof(float));
	close(file);
	exit(0);
}

int main() {
	srand( time( NULL ) );
	int ar_size = 1000000;
	float *ar = (float*) malloc(ar_size*sizeof(float)); //rzutowanie na float, bo malloc zwraca void*
	if(ar)
	{
		for (int i = 0; i<ar_size; i++)
		{
			ar[i] = RandomFloat(-1, 1);
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

	int status;
	float sum=0, average = 0;
	for(int i = 0; i<children; i++)
	{
		int child_pid;
		child_pid = waitpid(-1, &status, 0);
		//printf("Child pid: %d\n", child_pid);

		if (WIFEXITED(status))
		{
			int file;
			char buffer[25]; // buffer for filename
			snprintf(buffer, sizeof(buffer), "sum%d.txt", child_pid);
			//printf("%s\n", buffer);
			file = open(buffer, O_RDONLY);
			if (file == 0)
  		{
    		printf("\nUnable to open original file.\n");
    		return -1;
  		}
			ssize_t nread;
			char* child_sum[10];
			
  		nread = read(file, *child_sum, sizeof(float));
			if(nread == -1){
				int error = errno;
				printf("Read with error: %s\n", strerror(error));
			}
			//printf("Read %ld bytes.\n", nread);
			float read_sum = strtof (*child_sum, NULL);
			sum += read_sum;
			close(file);
		}
	}

	average = sum/children;
	printf("End average: %f\n", average);
	
	free(ar);
	return 0;
}