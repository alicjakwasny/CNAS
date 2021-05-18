#include <ctype.h> //toupper
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
	char tablica[100] = "czesc";
	char nowa[100] = {0};
	for (int i = 0; i<101; i++){
		nowa[i] = toupper(tablica[i]);
	}
	printf("%s", nowa);
	return 0;
}