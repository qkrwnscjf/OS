#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

	int i; 
	
	for( i = 0; i < 4; i++ )
		fork();

	sleep(60);

	return 0;
}


