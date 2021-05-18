#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <time.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#define M_PI   3.14159265358979323846264338327950288

void delay(int milli_seconds)
{
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

struct circular_buffer
{
  int size;
  int free_position;
  double array[3000];
};

int producer(int size)
{
	//open shared memory
	int oflag = O_RDWR;
	int info = shm_open("/shmem", oflag, 0777);
	if (info == -1)
  {
    perror ("shm_open: ");
    abort();
  }
	void *address;
	
	int prot = PROT_READ|PROT_WRITE;
	int flags = MAP_SHARED;
	address = mmap(NULL, (size_t) size, prot, flags, info, 0);
	if (address == (void *) -1)
  {
    perror ("mmap: ");
    abort();
  }
	//read the position of free position
	struct circular_buffer *buffer;
	buffer = address;
	//buffer = memcpy(buffer, address, (size_t) size);
	//collects the measurements
	//generation of sinus samples 
	double angle = 0;
	int f = 100; //signal frequency 100 Hz
	int fs = 2000; //sampling frequency
	double increment=(2*M_PI)/(fs/f);
	int i = 0;

  while(1)
  {
		while (angle<=(2*M_PI))
      {
        buffer->array[buffer->free_position]=sin(angle);
				//printf("%lf\n", buffer->array[buffer->free_position]);
				angle=i*increment;
				if(buffer->free_position == (sizeof(buffer->array)/sizeof(double)))
				{
					buffer->free_position = 0;
				}
				else
				{
					buffer->free_position ++;
				}
         usleep(1000000/fs);
				 i++;
      }
    angle = 0;
		i = 0;
  }
	return 0;
}

int consumer(int size)
{
	int oflag = O_RDWR;
	int info = shm_open("/shmem", oflag, 0777);
	if (info == -1)
  {
    perror ("shm_open: ");
    abort();
  }
	void *address;
	int prot = PROT_READ|PROT_WRITE;
	int flags = MAP_SHARED;
	address = mmap(NULL, (size_t) size, prot, flags, info, 0);
	if (address == (void *) -1)
  {
    perror ("mmap: ");
    abort();
  }
	//read the position of free position
	struct circular_buffer *buffer;
	buffer = address;

	while(1)
	{
		if(buffer->free_position == 0)
		{
		//	printf("%lf\n", buffer->array[size-1]);
		}
		else
		{
			//printf("%lf\n", buffer->array[(buffer->free_position - 1)]);
		}
    usleep(100000);
	}

	return 0;
}

int K_AVG(int size){
	int oflag = O_RDWR;
	int info = shm_open("/shmem", oflag, 0777);
	if (info == -1)
  {
    perror ("shm_open: ");
    abort();
  }
	void *address;
	int prot = PROT_READ|PROT_WRITE;
	int flags = MAP_SHARED;
	address = mmap(NULL, (size_t) size, prot, flags, info, 0);
	if (address == (void *) -1)
  {
    perror ("mmap: ");
    abort();
  }
	//read the position of free position
	struct circular_buffer *buffer;
	buffer = address;
	
	int init = 1;
	double sma = 0;
	double ema = 0;
	double old_ema = 0;
	double multiplier = 2;
	double sample = 0;
  printf("K_AVG\n");
	while(1)
	{

		if (init)
		{
			//filling the array to calculate SMA
			for(int i = 0; i < 10; i++){
				if(buffer->free_position == 0)
				{
					sma += buffer->array[size-1];
				}
				else
				{
					sma = buffer->array[buffer->free_position-1];
				}
			}
		sma = sma/10;
		printf("sma = %lf\n", sma);
		old_ema = sma;
		init = 0;
		
		}
		else{
			//adding new value
			if(buffer->free_position == 0)
			{
				sample = buffer->array[size-1];
				//printf("sample: %lf\n", sample);
				ema = (sample - old_ema)*multiplier + old_ema;
			}
			else
			{
				sample = buffer->array[buffer->free_position-1];
				//printf("sample: %lf\n", sample);
				ema = (sample - old_ema)*multiplier + old_ema;
			}
		}
		printf("EMA = %lf\n", ema);
		old_ema = ema;
    usleep(100000);
    printf("K_AVG\n");
	}
	return 0;
}

int K_F0(int size){
	int oflag = O_RDWR;
	int info = shm_open("/shmem", oflag, 0777);
	if (info == -1)
  {
    perror ("shm_open: ");
    abort();
  }
	void *address;
	int prot = PROT_READ|PROT_WRITE;
	int flags = MAP_SHARED;
	address = mmap(NULL, (size_t) size, prot, flags, info, 0);
	if (address == (void *) -1)
  {
    perror ("mmap: ");
    abort();
  }
	//read the position of free position
	struct circular_buffer *buffer;
	buffer = address;
	//buffer = memcpy(buffer, address, (size_t) sizeof(buffer));
	int counter = 0;
	int frequency = 0;
  int samples=2000;
	while(1)
	{
		// zmierzyć start time
		for(int i = 0; i<samples; i++)
		{
			if(buffer->free_position == 0 && buffer->array[size-1] <0 && buffer->array[size-2])
			{
        //printf("pierwszy if");
				counter++;
			}
			else if (buffer->array[(buffer->free_position - 1)] < 0 && buffer->array[(buffer->free_position - 2)] > 0)
			{
        //printf("drugi if");
				counter++;
			}

      usleep(1000000/samples);
		}
			frequency = counter;
      printf("Estimated frequency: %d\n", frequency);
			counter = 0;
	}
	return 0;
}

int main(void)
{
	//create circular buffer
	struct circular_buffer buffer;
	buffer.size = sizeof(buffer.array);
	buffer.free_position = buffer.array[0];

	//create shared memory
	int oflag = O_RDWR | O_CREAT;
	int fd = shm_open("/shmem", oflag, 0777);
	if (fd == -1)
  {
    perror ("shm_open: ");
    abort();
  }

	int size = sizeof(buffer);
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

	//fork
	pid_t pid;
  pid = fork();

  if (pid == -1)//checking for errors
  {
    perror("Fork: ");
    exit(1);
  }

  if ( pid == 0 )
	{
		K_F0(sizeof(buffer));
    //K_AVG(buffer.size); 
	}

	producer(sizeof(buffer));

	return 0;
}
