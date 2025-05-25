#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN 100
#define HISTORY_LEN 5

/* Doubly Circular Linked List Node */
//F. 히스토리 기능 추가 : 명령어 입력 히스토리, 출력, 삭제 및 출력 히스토리를 구현한다.
typedef struct Node {
    char command[MAX_LEN]; //명령어 문자열
    struct Node* prev; // 이전 노드 
    struct Node* next; // 다음 노드
} Node;

Node* history_head = NULL;
Node* history_tail = NULL;
int history_size = 0;

/* 명령어 히스토리 추가 */
void add_history(const char* cmd) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    
    // 만약 명령어가 빈 문자열이라면 (단지 Enter만 눌린 경우)
    if (cmd[0] == '\n') {
        strcpy(new_node->command, "");  // 빈 명령어를 하나로 취급
    } else {
        strncpy(new_node->command, cmd, MAX_LEN); //일반 명령어 저장
    }

    new_node->command[MAX_LEN - 1] = '\0'; // 종료 보장
	//첫 노드일 경우
    if (history_head == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        history_head = history_tail = new_node;
    } else {
    	// 리스트에 새 노드 추가 (tail 뒤에 삽입)
        new_node->prev = history_tail;
        new_node->next = history_head;
        history_tail->next = new_node;
        history_head->prev = new_node;
        history_tail = new_node; // tail 업데이트 (가장 최근)
    }
	
	// 히스토리 크기 조절
    if (history_size < HISTORY_LEN) {
        history_size++;
    } else {
        // 히스토리가 꽉 차면, 가장 오래된 명령어 삭제
        Node* to_delete = history_head;
        history_head = history_head->next;
        history_head->prev = history_tail;
        history_tail->next = history_head;

        // 삭제된 명령어 출력
        printf("Delete cmd : %s\n", to_delete->command);

        free(to_delete);
    }
}



// 히스토리 출력 함수 print_history()
void print_history() {
    if (!history_head) {
        printf("No history.\n");
        return;
    }

    Node* temp = history_head;
    for (int i = 0; i < history_size; i++) {
        temp->command[strcspn(temp->command, "\n")] = '\0'; // \n 제거
        printf("%d\t%s\n", i, temp->command); //번호 및 명령어 출력
        temp = temp->next;
    }
    
  
}

// 히스토리 삭제 및 전체 출력
void delete_history() {
    if (!history_head) return;

    printf("Delete cmd history list :\n");

    Node* current = history_head;
    for (int i = 0; i < history_size; i++) {
                
        current->command[strcspn(current->command, "\n")] = '\0'; // \n 제거

        // 빈 문자열도 출력
        if (strlen(current->command) == 0) {
            printf("Deleted cmd : \n");
        } else {
            printf("Deleted cmd : %s\n", current->command);
        }

        Node* temp = current;
        current = current->next;
        free(temp);
    }

    history_head = history_tail = NULL;
    history_size = 0;

    printf("Finish !\n");
}




int main(void) {
    char* args[MAX_LEN / 2 + 1];
    int should_run = 1;
    int background = 0;
    
    char *input;
    int status;

    while (should_run) {
        printf("myshell> ");
        fflush(stdout);
	
	// A.사용자에게 문자열(shell 명령어)를 입력받는다. (fgets사용)쉘 명령어 입력 받기
        input = (char*)malloc(MAX_LEN * sizeof(char));
        fgets(input, MAX_LEN, stdin);
        
        /*if (strcmp(input, "\n") == 0) {
            add_history("");
            free(input);
            continue;
        }*/

        add_history(input);  // 무조건 히스토리 저장 (Enter 포함)

        // B.입력받은 문자열을“ \n" 단위로쪼갠다. (strtok사용)
        //만약, 입력받은 문자열이 비어있다면, goto문을 써서 while문내에서 마지막으로 jump시킨다.
        int i = 0;
        args[0] = strtok(input, " \n");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " \n");
        }

        if (args[0] == NULL) {
            goto no_input; //goto 사용
        }

        // C.쪼개진 문자열을 비교한다.(strcmp사용), 쪼개진 문자열이 "exit"와 같다면, 프로그램을종료시킨다.
        if (strcmp(args[0], "exit") == 0) {
            delete_history();
            free(input);
            break;
        }

        // F. history 출력 
        if (strcmp(args[0], "history") == 0) {
            print_history();
            free(input);
            continue;
        }

        // C. 쪼개진 문자열이 "&" 로 마무리 된다면, background 프로세스로 실행하기 위해 flag를 활성화한다.
        if (strcmp(args[i - 1], "&") == 0) {
            args[i - 1] = NULL;
            background = 1;
        }

        // E.디렉터리 이동이 가능해져야 한다. (chdir() 사용)
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                char* home = getenv("HOME");
                if (home != NULL) chdir(home);
            } else if (chdir(args[1]) != 0) {
                perror("cd error");
            }
            free(input);
            continue;
        }

        // D.현재 process의 자식프로세스를 생성한다. (fork 사용)
        pid_t pid = fork();
        if (pid < 0) {
            perror("error"); // 만약 pid가 음수이면, fork 과정에 문제가 발생한것임으로 error를 출력하고 빠져나온다.
            exit(1);
        }
        if (pid == 0) { // 만약 pid가 0이면,자식프로세스를 실행시킨다.
            if (execvp(args[0], args) < 0) {
                printf("command error, not found.\n");
                exit(1);
            }
        }
        if (pid > 0) { // 자식프로세스가 쪼개진 문자열(명령한줄)을 수행한다. (execvp사용) 만약pid가0보다크면,부모프로세스를실행시킨다.
            if (!background) {
                printf("waiting for child, not a background process\n");
		        waitpid(pid, &status, 0); // 만약 background flag가 활성화 되어있지 않다면, 자식프로세스의 종료를 기다린다.(waitpid사용)
		        printf("child process complete\n");
            } else {
                printf("background process\n");
                background = 0;
                waitpid(pid, &status, WNOHANG);  // 그렇지 않다면, 부모프로세스(프로그램)를 바로 종료 시킨다.
            }
        }
        no_input:;
        free(input);
    }

    return 0;
}
