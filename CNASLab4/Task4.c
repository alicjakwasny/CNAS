/*Modify task 1.8 to use pipes to pass return values from child processes to their parent.
You may want to create a separate pipe for each child (or deal with buffering issues...)
*/
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

double Double(double min, double max) 
{
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = max - min;
    double r = random * diff;
    return min + r;
}

int child_task(double *array, int a, int pipe) 
{
  int ar_size = 1000000;
  int children = 10;
  int num=ar_size/children;
  double sum;
  
  for (int i = a; i < a + num; i++ ) 
  {
    sum += array[i];
  }
  write(pipe, &sum, sizeof(sum));
  close(pipe);
  
  return 0;
}

double average(double *array) 
{
  int children = 10;
  double sum = 0;
  for (int i = 0; i < children; i++) 
  {
    sum += array[i];
  }
  printf("sum: %f\n", sum);
  return sum/children;
}

int main() 
{
  int ar_size = 1000000;
  int children = 10;
  int num=ar_size/children;
  int ch_pids[children];
    
  pid_t pid;
    
  double sums[10] = {0};
  double * array = malloc(ar_size * sizeof(double));

  srand(time(NULL));

	if(array)
	{
		for (int i = 0; i<ar_size; i++)
		{
			array[i] = Double(-1, 1);
		}
	}
  int child_to_parent[children][2];

  for (int i = 0; i < children; i++) 
  {
    if (pipe(child_to_parent[i]) < 0 )
    {
      fprintf(stderr,"Pipe error!\n");
      return -1;
    }
    pid = fork();
    if (pid == -1)
    {
      exit(1);
    }
    if (pid == 0)
    {     
      child_task(array, (int)(i*num), child_to_parent[i][1]);
      exit(0);
    }
    ch_pids[i] = pid;
  }

  for (int i = 0; i < children; i++) 
  {
    read(child_to_parent[i][0], &sums[i], sizeof(double));    
    close(child_to_parent[i][0]);
  }
  printf("average: %f\n", average(sums));
  
  return 0;
}
