/*Perform basic test with pipe in single process
1) create a pipe
2) write some data
3) read it back
4) verify
*/

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//to compile: clang-7 -pthread -o Task1 Task1.c

int main() {
	int pipefd[2];
	int pipe(int pipefd[2]);
  int size = 5;
	char* message = "Hello";
	write(pipefd[1], message, size);
	char read_message[size];
	read(pipefd[0], read_message, size);
  printf("%s\n", read_message);
	return 0;
}