//to compile: clang-7 -pthread -o main main.c -lrt -lm -fsanitize=address


#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <time.h>
#include <math.h>

struct circular_buffer
{
  int size;
  int free_position;
  int reader_postion;
  int writer_position;
  double array[200];
};

int process_A(int a)
{
		struct circular_buffer buffer_a;
    
    for (int i=0; i<200;i++ )
    {
      buffer_a.array[i]=sin(i/10);  
    }
	return buffer_a.array[a];
}

int consumer()
{
	//wants to access data
	int fd = shm_open("/shmem", MAP_SHARED, 0777);
  struct circular_buffer buffer_consumer;
  buffer_consumer.size=200;
	if (fd == -1)
  {
  perror ("shm_open: ");
  abort();
  }
	void *child_message;
	child_message = mmap(NULL, (size_t) buffer_consumer.size, PROT_READ|PROT_WRITE, MAP_SHARED  | MAP_ANONYMOUS, fd, 0);
	if (child_message == (void *) -1)
  {
    perror ("mmap: consumer");
    abort();
  }
  printf("%s\n",child_message);
	exit(0);
	return 0;
}

int producer()
{
	//writes samples to the array to the current free position

  struct circular_buffer buffer_producer, *ptr;
  buffer_producer.free_position=0;
  buffer_producer.size=200;
  for(int i=1;i<1000;i++)
  {
    buffer_producer.array[buffer_producer.free_position]=process_A(buffer_producer.free_position);
    buffer_producer.free_position++;
    if(buffer_producer.free_position==buffer_producer.size)
    {
      buffer_producer.free_position=0;
    }
  } 
  //write to shm
  int oflag = O_RDWR | O_CREAT;
	int fd = shm_open("/shmem", oflag, 0777);
  void *address;
	int prot = PROT_READ|PROT_WRITE;
	int flags = MAP_SHARED | MAP_ANONYMOUS;
  //address = mmap(NULL, (size_t) buffer_producer.size, prot, flags, fd, 0);
  ptr=mmap(NULL, (size_t) buffer_producer.size, prot, flags, fd, 0);

	//memcpy(address, (const void *restrict) &buffer_producer.array, (size_t) buffer_producer.size);
	return 0;
}

int main(void) 
{
  
	int oflag = O_RDWR | O_CREAT;
	int fd = shm_open("/shmem", oflag, 0777);
	if (fd == -1)
  {
    perror ("shm_open: ");
    abort();
  }

	int size = 100;
	int ftr = ftruncate(fd, size);
	if (ftr == -1)
  {
    perror ("ftruncate: ");
    abort();
  }

	void *address;
	int prot = PROT_READ|PROT_WRITE;
	int flags = MAP_SHARED;
	address = mmap(NULL, (size_t) size, prot, flags, fd, 0);
	if (address == (void *) -1)
  {
    perror ("mmap: ");
    abort();
  }

	pid_t pid; 
  pid = fork();
 
  if (pid == -1)
  {
    perror("Fork: ");
    exit(1);
  }

  if ( pid ==0 )
  {

  consumer();
  }
  
  producer();


	
  return 0;
}