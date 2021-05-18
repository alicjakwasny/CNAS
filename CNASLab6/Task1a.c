/*We have a "difficult" problem to solve - we need to compute a large number of integer sums (for this task we may assume, that computing a+b is an expensive operation that takes at least 1 second to complete).
Your objective is to write two programs (it may be one program that spawns child processes as required or two separate programs):
- master (producer) - it will generate tasks and will place them in a queue to be executed
- slave (consumer) - it will execute the task and ( OPTIONAL: send the result back to the producer )
Each task should be described by a structure (common for both producer and consumer). This structure must contain:
- PID of the producer process
- task sequence (id) number
- task data (for example - two integer values A and B)
The number of tasks to solve is given as a runtime parameter for a producer.
The tasks are generated randomly.
The consumer works in an infinite loop. It reads tasks from the queue and executes them. In order to simulate the "difficulty" of the problem, the consumer pretends that the calculation of the sum of a+b takes at least a few seconds. After calculating the result the consumer displays on the screen:
- all the task data (producer's PID, A and B)
- his PID
- result
(OPTIONAL: instead of displaying the result - send it back to producer; hint1 - define a structure that holds the result; hint2 - do not use the same queue)
Test situations in which you run:
A) one producer and one consumer
B) one producer and several consumers
C) several producers and one consumer
D) several producers and several consumers */

//to compile: clang-7 -pthread -o Task1a Task1a.c -lrt

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>     		/* srand, rand */
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

struct task_msg  
{
    int pid; //pid of the master process
    int task_id; //number of task
    int A; //data for the task
    int B;
};

int task(int A, int B)
{
	sleep(3); //pretend that the task is time consuming
	return (A+B);
}

void master_task(int tasks, mqd_t qfd) //tasks - number of tasks
{
	for (int i = 0; i<tasks; i++)
	{
		int A = rand()%10;//generate random task (A, B)
		int B = rand()%10;
		//put the data into structure: PID of the process number, task sequence, task data
		struct task_msg task_data;
		task_data.pid = getpid();
		task_data.task_id = i;
		task_data.A = A;
		task_data.B = B;
		//put the data into queue
		int rv = mq_send (qfd, (const char *)&task_data, sizeof(task_data), 0);
  	// int rv = mq_send (qfd, reinterpret_cast<const char *>(&m1), sizeof(m1), 0);
  	if (rv == -1)
  	{
    	perror ("mq_send: ");
    	abort();
  	}
	}
}

void slave_task(mqd_t qfd)
{
	
	//infinite loop
	while(1)
	{
	//read task from the queue
	int size = sizeof(struct task_msg);
	//char buf[size];
	struct task_msg task_data;
  int rv = mq_receive(qfd, (char *)&task_data, size, NULL);
  if (rv == -1)
  {
    perror ("mq_receive: ");
    abort();
  }
	//execute task
	int sum = task(task_data.A, task_data.B);
	//display result on the screen: all the task data, PID of slave, result
	printf("Task data:\nMaster's pid: %d\nSlave's pid: %d\nTask id: %d\nA: %d\nB: %d\nResult: %d\n\n", task_data.pid, getpid(), task_data.task_id, task_data.A, task_data.B, sum);
	}
}

void spawn_master(int tasks_number, mqd_t qfd)
{
	//spawn a child process that does master_task
	pid_t pid;
  pid = fork();

  if (pid == -1)//checking for errors
  {
    perror("Fork: ");
    exit(1);
  }

  if ( pid == 0 )
  {
    master_task(tasks_number, qfd);
		exit(0);
  }
}

void spawn_slave(mqd_t qfd)
{
	//spawn a child process that does slave_task
	pid_t pid;
  pid = fork();

  if (pid == -1)//checking for errors
  {
    perror("Fork: ");
    exit(1);
  }

  if ( pid == 0 )
  {
    slave_task(qfd);
		exit(0);
  }
}

int main(int argc, char **argv)
{
	srand( time( NULL ) );

  int oflag = O_RDWR | O_CREAT;
  struct mq_attr attrs;
  attrs.mq_flags = 0;
  attrs.mq_maxmsg = 8;
  attrs.mq_msgsize = sizeof( struct task_msg ); //   100;
  attrs.mq_curmsgs = 0;
  //mq_unlink("/myqueue");
  mqd_t qfd = mq_open("/myqueue", oflag, 0777, &attrs);
  if (qfd == (mqd_t) -1)
  {
      perror ("mq_open: ");
      abort();
  }
  
	//Scenario 1: 1 producer and 1 consumer
	/*
	int tasks_number = 5;

	spawn_master(tasks_number, qfd);
	spawn_slave(qfd);
	*/

	//Scenario 2: 1 producer and several consumers
	/*
	int tasks_number = 10;
	int slaves_number = 3;

	spawn_master(tasks_number, qfd);

	for(int i = 0; i<slaves_number; i++)
	{
		spawn_slave(qfd);
	}
	*/

	//Scenario 3: several producers and 1 consumer
	/*
	int tasks_number = 5;
	int master_number = 3;

	spawn_slave(qfd);

	for(int i = 0; i<master_number; i++)
	{
		spawn_master(tasks_number, qfd);
	}
	*/
	//Scenario 4: several producers and several consumers
	///*
	int tasks_number = 10;
	int master_number = 3;
	int slaves_number = 3;

	for(int i = 0; i<master_number; i++)
	{
		spawn_master(tasks_number, qfd);
	}

	for(int i = 0; i<slaves_number; i++)
	{
		spawn_slave(qfd);
	}
	//*/
  return 0;
}