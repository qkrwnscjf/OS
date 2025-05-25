#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>

int main(void) {

	int status;
	int cnum;
	pid_t pid; 

	pid = fork();

	if( pid < 0 ) {
		perror("Fork");
		exit(1);
	} else if( pid == 0 ) {
		ptrace(PTRACE_TRACEME, pid, 0, 0);
		raise(SIGSTOP);
		syscall(463);
	} else {
		while(1) {
			wait(&status);
			if( WIFEXITED(status) )
				break;
			cnum = ptrace(PTRACE_PEEKUSER, pid, 8 * ORIG_RAX, 0); 
			printf("SYSCALL: %d\n", cnum);
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
		}
	}

	return 0;
}
