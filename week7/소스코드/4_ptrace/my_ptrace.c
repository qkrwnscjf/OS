#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/reg.h>

#define syscode_case(x)	\
case x:			\
	return #x

const char *get_syscode(long);

struct user_regs_struct regs;

int main(void) {

	int status;
	pid_t pid;

	pid = fork();

	if( pid < 0 ) {
		perror("fork");
		exit(1);
	} else if( pid == 0 ) {
		/* TODO 1: 자식 프로세스를 추척 가능하도록 설정 */
		ptrace(PTRACE_TRACEME,0, 0, 0); //Req: PTRACE_TRACEME
		execl("/bin/ls", "ls", NULL);
	} else {
		while( waitpid(pid, &status, 0) && !WIFEXITED(status) ) {
		/* TODO 2: 전체 레지스터 값을 가져오기 */
			ptrace(PTRACE_GETREGS, pid, 0, &regs); // Req : PTRACE_GETREGS
			fprintf(stderr, "[SYSCALL]:%-20s\t%5lld\n",
						get_syscode(regs.orig_rax),
						regs.orig_rax);
		/* TODO 3: 매 시스템 콜 마다 실행을 중단하게 끔 설정 */
		ptrace(PTRACE_SYSCALL, pid, 0, 0); //Req : PTRACE_SYSCALL
		}	
	}

	return 0;
}

const char *get_syscode(long code) {
	switch( code ) {
		syscode_case(SYS_read);
		syscode_case(SYS_write);
		syscode_case(SYS_close);
		syscode_case(SYS_fstat);
		syscode_case(SYS_mmap);
		syscode_case(SYS_mprotect);
		syscode_case(SYS_munmap);
		syscode_case(SYS_brk);
		syscode_case(SYS_rt_sigaction);
		syscode_case(SYS_rt_sigprocmask);
		syscode_case(SYS_rt_sigreturn);
		syscode_case(SYS_ioctl);
		syscode_case(SYS_getpid);
		syscode_case(SYS_pread64);
		syscode_case(SYS_access);
		syscode_case(SYS_pipe);
		syscode_case(SYS_execve);
		syscode_case(SYS_getdents);
		syscode_case(SYS_statfs);
		syscode_case(SYS_exit_group);
		syscode_case(SYS_openat);
		syscode_case(SYS_mkdirat);
		syscode_case(SYS_prlimit64);
		syscode_case(SYS_arch_prctl);
		syscode_case(SYS_set_tid_address);
		syscode_case(SYS_set_robust_list);
		default: 
		return "Not registered in the function list.";
	}
}
