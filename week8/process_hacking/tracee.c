#include <stdio.h>
#include <sys/ptrace.h>

int pw = 0xabcd;

int main(void) {

	int i;
	while( pw == 0xabcd )
		i++;

	printf("[pw]: %x \n", pw);
	printf("Exit tracee process!\n");

	return i;
}


