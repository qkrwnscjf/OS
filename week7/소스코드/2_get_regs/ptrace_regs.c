#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>

int main(void) {

	struct user_regs_struct regs;
	int insyscall = 0;
	long orig_rax, rax;
	int status;
	pid_t pid;

	pid = fork();

	if( pid < 0 ) {
		perror("fork");
		exit(1);
	} else if( pid == 0 ) {
		ptrace(PTRACE_TRACEME, pid, 0, 0);
		execl("/bin/ls", "ls", NULL);
	} else {
		while(1) {
			wait(&status);
			if( WIFEXITED(status) )
				break;
			orig_rax = ptrace(PTRACE_PEEKUSER, pid, 8 * ORIG_RAX, 0);
			if( orig_rax == SYS_write )  {
				if( insyscall == 0 ) {
					insyscall = 1;
					ptrace(PTRACE_GETREGS, pid, 0, &regs);
					printf("Write called with "
						"%lld, %lld, %lld\n",
						regs.rdi, regs.rsi, regs.rdx);
				} else {
					insyscall = 0;
					rax = ptrace(PTRACE_PEEKUSER, pid, 8 * RAX, 0);
					printf("Write returned "
						"with %ld\n", rax);
				}
			}

			ptrace(PTRACE_SYSCALL, pid, 0, 0);
		}
	}

	return 0;
}
