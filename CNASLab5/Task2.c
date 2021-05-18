/* Use dup() system call to create a "fake" stdout.

After duplicating stdout you should be able to "print" data by writting to this duplicate file descriptor. */

//to compile: clang-7 -pthread -o Task2 Task2.c

#include <sys/types.h>
#include<unistd.h> //dup2
#include<stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include<errno.h>
#include<string.h>

int main() {
char message[11]="adsfff\n";
int soutfd = dup (1);
write (soutfd, message, sizeof(message)) ;

FILE *fakeout = fdopen(soutfd, message);

//printf("Hi!\n"); //usng stdout by default
//fprintf(stdout, "Hello!\n"); //using fprintf to indicate a specific stream, here - stdout
fprintf(fakeout, "Hey!\n"); //writing to fakeout but should also print to stdout
return 0;
}