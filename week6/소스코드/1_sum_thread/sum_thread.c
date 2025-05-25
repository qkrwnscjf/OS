#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum;
void *runner(void *param);

int main(int argc, char *argv[]) {
	
	pthread_t tid;

	if( argc != 2 ) {
		fprintf(stderr, "Usage: ./sum_pthread <integer value> \n");
		exit(1);
	}

	if( atoi(argv[1]) < 0 ) {
		fprintf(stderr, "Input value : [%d] must be >= 0 \n", atoi(argv[1]) );
		exit(1);
	}

	pthread_create(&tid, NULL, runner, argv[1]);

	pthread_join(tid, NULL);

	printf("sum = %d\n", sum);

	return 0;
}

void *runner(void *param) {
	int i;
	char *str = (char *)param; 
	int upper = atoi(str);
	sum = 0;

	for( i = 0; i <= upper; i++ )
		sum += i;
	
	pthread_exit(NULL);
}
