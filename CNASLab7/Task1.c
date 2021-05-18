//to compile: clang-7 -pthread -o main main.c -lrt

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>

int main(void) {
  
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

	char text[] = "hello";
  //write to shm
	memcpy(address, (const void *restrict) &text, (size_t) sizeof(text));

	pid_t pid; 
  pid = fork();
 
  if (pid == -1)
  {
    perror("Fork: ");
    exit(1);
  }

  if ( pid ==0 )
  {
    //read from shm
    int fd = shm_open("/shmem", oflag, 0777);
		if (fd == -1)
  	{
    perror ("shm_open: ");
    abort();
  	}
		void *child_message;
		child_message = mmap(NULL, (size_t) size, prot, flags, fd, 0);
		if (child_message == (void *) -1)
  	{
    	perror ("mmap: ");
    	abort();
  	}
    printf("%s\n",child_message);

		exit(0);
  }

  return 0;
}