#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include "util.c"

struct user_regs_struct regs;

int main(int argc, char *argv[]) {
	
	int wstatus;
	int keep_loop = 1;
	pid_t tracee_pid; 

	if( argc < 2 ) {
		printf("[USAGE]: ./tracer <pid-of-target-process> \n");
		return -1;
	}
	
	if( atoi(argv[1]) < 0 ) {
		printf("[ERROR]: Illegal PID number \n");
		return -1;
	}

	tracee_pid = atoi(argv[1]);
	ptrace(PTRACE_ATTACH, tracee_pid, 0, 0);
	waitpid(tracee_pid, &wstatus, 0);

	while( keep_loop ) {
		ptrace(PTRACE_SINGLESTEP, tracee_pid, 0, 0);
		waitpid(tracee_pid, &wstatus, 0);
		ptrace(PTRACE_GETREGS, tracee_pid, 0, &regs);
		keep_loop = pokepeek_iteration(tracee_pid, regs);
	}

	ptrace(PTRACE_DETACH, tracee_pid, 0, 0);

	return 0;
}
