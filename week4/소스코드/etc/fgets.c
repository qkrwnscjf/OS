#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 100 

int main(void) {

	char *input = (char *)malloc(MAX_LEN * sizeof(char));

	fgets(input, MAX_LEN, stdin);

	printf("INPUT: %s\n", input);

	free(input);

	return 0;
}
