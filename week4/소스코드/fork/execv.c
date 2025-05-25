#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {

	pid_t pid;

	char *const paramList[] = 
		{"/bin/ls", "-l", ".", NULL};
	if( (pid = fork()) == -1 ) {
		perror("Fork Error");
	}
	else if( pid == 0 ) {
		execv("/bin/ls", paramList);
	} else {
		wait(NULL);
	}

	return 0;
}


