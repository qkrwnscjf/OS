#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h> // bool 타입 사용을 위해 추가

#define NUM_STUDENTS 5 // student (N)는 5명으로 설정, 필요 시 변경 가능
#define MAX_CHAIRS 3   
#define MAX_TIME 10    // 프로그램 실행 최대 시간 

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KBLU "\x1B[34m"

int waiting_chairs = 0; // 현재 남아있는 대기 의자 수
int next_seat = 0;      // 다음 student 앉을 의자 순서
int next_teach = 0;     // TA가 teach 할 학생의 순서
int waiting_queue[MAX_CHAIRS]; // 대기 중인 학생을 저장
bool terminate = false; // 프로그램 종료 플래그 

pthread_mutex_t mutex;  

sem_t students_sem;     // student 세마포
sem_t ta_sem;           // TA 세마포

void *student(void *_name) { 
    int stud = *((int *)_name); // 학생의 순번

    while (!terminate) { 
        printf("%dth student: do programming assignment\n", stud);
        sleep(rand() % 3 + 1); 

        pthread_mutex_lock(&mutex); // critical section 전 mutex 잠금
        if (waiting_chairs < MAX_CHAIRS && !terminate) {
            waiting_queue[(next_seat + 1) % MAX_CHAIRS] = stud;
            waiting_chairs++;
            printf("%dth student: sat on a chair waiting for the TA to finish.\n", stud);
            sem_post(&students_sem); //student 도착 알림
            pthread_mutex_unlock(&mutex);

            sem_wait(&ta_sem); // TA가 부를 때까지 대기
        } else { // 의자가 꽉 찼을 경우 또는 종료 신호
            pthread_mutex_unlock(&mutex); // mutex 해제
            if (terminate) break; // 루프 종료
            printf("%dth student: no chair available, will return later.\n", stud);
        }

        sleep(1); 
    }

    pthread_exit(NULL); 
}

void *ta(void *_name) { 
    while (!terminate) {
        sem_wait(&students_sem); // student 대기 wait

        pthread_mutex_lock(&mutex); // critical section 전 mutex 잠금
        if (waiting_chairs > 0 && !terminate) {
            int stud = waiting_queue[(next_teach + 1) % MAX_CHAIRS];
            waiting_chairs--;
            printf("TA: Get next student on the chair\n");
            pthread_mutex_unlock(&mutex);

            printf("TA: Help student finished assignment ...\n");
            sleep(2); // student 도움 여유 시간
            sem_post(&ta_sem); // 도움 완료 후 student에게 신호
        } else { // 도울 student 없을 시
            pthread_mutex_unlock(&mutex);
            if (terminate) break; // 루프 종료
        }
    }

    pthread_exit(NULL); // 
}

int main() {
    pthread_t ta_thread;
    pthread_t student_threads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];

    pthread_mutex_init(&mutex, NULL);  // mutex 초기화
    sem_init(&students_sem, 0, 0);     
    sem_init(&ta_sem, 0, 0);           

    pthread_create(&ta_thread, NULL, ta, NULL); // TA 스레드 생성

    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i;
        pthread_create(&student_threads[i], NULL, student, &student_ids[i]); // student 스레드 생성
    }

    sleep(MAX_TIME); // 최대 실행 시간
    terminate = true; // 종료 플래그

    pthread_join(ta_thread, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(student_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex); 
    sem_destroy(&students_sem);    
    sem_destroy(&ta_sem);          

    
    return 0;
}
