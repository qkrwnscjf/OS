#include <stdio.h>
#include <string.h>

int main(void) {

	char str[] = "This is a sample string, just testing.";
	
	char *p; 

	printf("str[] = \"%s\"\n", str);

	p = strtok(str, " ");

	while( p != NULL ) {
		printf("%s\n", p);
		p = strtok(NULL, " ,");
	}

	printf("str[] = \"%s\"\n", str);

	return 0;
}
