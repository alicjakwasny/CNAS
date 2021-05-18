#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

float RandomFloat(float min, float max) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

int main()
{
	for(int i = 0; i< 15; i++){
		printf("%f\n", RandomFloat(-1.0, 1.0));
	}
	return 0;
}