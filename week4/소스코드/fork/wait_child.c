#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {

	pid_t pid;
	int retval;
	int status;

	pid = fork();

	if( pid < 0 ) {
		perror("Fork Error:");
		exit(0);
	}

	if( pid == 0 ) {
		// 자식 프로세스
		int i;

		for( i = 0; i < 10; i++ ) {
			printf("Child: %d\n", i);
			sleep(3);
		}
		exit(3);
	} else {
		// 부모프로세스는 
		// 자식 프로세스가 종료할 때까지 기다린다
		printf("Waitting for Child process (pid: %d) \n", pid);
		retval = waitpid(pid, &status, 0);
		printf("Child exits (status: %d) \n", status);
		// status 값만 가지고 
		// 어떻게 종료되어는지 알 수 없으므로, 
		// 다음과 같은 매크로 함수를 통해 검사해야 한다
		
		// 자식 프로세스가 어떤 정보를 반환하며
		// 종료되어는지 확인한다 
		if( retval > 0 ) {
			if( WIFEXITED(status) ) {
				// 자식 프로세스가 정상적으로 
				// 종료 되었을 때
				printf("Child exited by process completetion: %d\n",
							WEXITSTATUS(status) );
			}
			if( WIFSIGNALED(status) ) {
				// 자식 프로세스가 시그널에 
				// 의해 종료되었을 때
				printf("Child exited by signal : %d\n", 
							WTERMSIG(status) );
			}
		} else {
			// 자식 프로세스 에러가 발생한 경우
			printf("Unexpected error \n");
			exit(0);
		}
	}	

	return 0;
}
