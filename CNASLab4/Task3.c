/*Create a simple "echo server"

The child in an infinite loop:
receives data from the parent (line of text, you can assume it is not longer than 8000 characters)
converts all lowercase letters into uppercase (you can use the toupper function)
sends the result back to the parent

In the parent process, test sending several texts to the child, read and display the answers
*/
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h> //toupper
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void child_task(int parent_to_child, int child_to_parent) 
{
  while(1) 
  {
    char data[8000] = {0};
		char uppercase[8000] = {0};
    int read_data = read(parent_to_child, &data, sizeof(data));
		if(read_data>0)
    {
			for(int i=0;i<read_data;i++)
			{      
				uppercase[i] = toupper(data[i]);
			}
			int write_data = write(child_to_parent, &uppercase, read_data);
		}	
  }
}

int main() 
{
  int child_to_parent[2];
  int parent_to_child[2];
  
  pipe(child_to_parent);
  pipe(parent_to_child);

  int status;
	pid_t pid;
  pid = fork();

  if (pid == -1) 
  {    
    exit(1);
  }

  if (pid == 0) //child task
  {
    child_task(parent_to_child[0], child_to_parent[1]);
    exit(0);
  }

  char data[8000];
  char messages[3][60] = {"hi i'm a child", "this is lowercase", "nice"};

  for (int i = 0; i < 3; ++i) 
  {
    write(parent_to_child[1], &messages[i], sizeof(messages[i]));
    read(child_to_parent[0], data, sizeof(data));
    printf("%s\n", data);
  }
	
	wait(&status);
	close(parent_to_child[1]);
  close(child_to_parent[0]);
	
	return 0;
}