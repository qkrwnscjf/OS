#include <stdio.h>
#include <string.h>

int main(void) {

	char src[50];
	char dest[100];

	memset(dest, '\0', sizeof(dest));
	
	strcpy(src, "Operating System are Amazing, aren't they?");

	printf("Before strcpy:\nSRC: %s\nDEST: %s\n\n", src, dest);

	strcpy(dest, src);

	printf("After strcpy:\nSRC: %s\nDEST: %s\n\n", src, dest);

	return 0;
}
