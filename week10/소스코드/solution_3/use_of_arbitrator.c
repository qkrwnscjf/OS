#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define INIT 0
#define EATING 1
#define THINKING 2

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"

#define N 5

int name[N];
int state[N];

sem_t forks[N]; // Binary Semaphore == Mutex

pthread_mutex_t print_mutex;

/* Declare waiter */
pthread_mutex_t mutex;

void print_table_index() {
    for (int i = 0; i < N; i++)
	printf("=========================");
    printf("\n");
    for (int i = 0; i < N; i++)
	printf("|\tPHIL[%d]\t\t|", i);
    printf("\n");
    for (int i = 0; i < N; i++)
	printf("=========================");
    printf("\n");
}

void print_fork(int name, int fork, char *action) {
    pthread_mutex_lock(&print_mutex);
    printf("\tPHIL[%d] %s %d th fork.\n", name, action, fork);
    pthread_mutex_unlock(&print_mutex);
}

void print_arbitrator(int phil, char *action) {
    pthread_mutex_lock(&print_mutex);
    printf("The waiter %s PHIL[%d]\n", action, phil);
    pthread_mutex_unlock(&print_mutex);
}

void update_state(int phil, int status) {
    pthread_mutex_lock(&print_mutex);
    state[phil] = status;
    for (int i = 0; i < N; i++) {
	if (state[i] == INIT)
	    printf("|\tINIT\t\t|");
	else if (state[i] == THINKING)
	    printf("|%s\tTHINKING\t%s|", KBLU, KNRM);
	else if (state[i] == EATING)
	    printf("|%s\tEATING\t\t%s|", KGRN, KNRM);
    }
    printf("\n");
    pthread_mutex_unlock(&print_mutex);
}

void *philosopher(void *_name) {
    int phil = *((int *)_name);

    do {
	/* Wait for the waiter */
	pthread_mutex_lock(&mutex);
	print_arbitrator(phil, "serves");

	sem_wait(&forks[(phil + 1) % N]);
	print_fork(phil, (phil + 1) % N, "take");
	sem_wait(&forks[phil]);
	print_fork(phil, phil, "take");

	/* The waiter leaves */
	print_arbitrator(phil, "leaves");
	pthread_mutex_unlock(&mutex);

	update_state(phil, EATING);

	sleep(1);

	update_state(phil, THINKING);

	print_fork(phil, (phil + 1) % N, "put");
	sem_post(&forks[(phil + 1) % N]);
	print_fork(phil, phil, "put");
	sem_post(&forks[phil]);

	sleep(1);
    } while (1);
}

int main() {
    pthread_t thread_id[N];

    pthread_mutex_init(&print_mutex, NULL);

    /* Initialize the waiter */
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < N; i++) {
	/* Initialize each forks with 1 */
	sem_init(&forks[i], 0,
		1); // Assign semaphore as a binary value (0 or 1) (== mutex)
	state[i] = 0;
    }

    print_table_index();

    for (int i = 0; i < N; i++) {
	name[i] = i;
	pthread_create(&thread_id[i], NULL, philosopher, &name[i]);
    }

    for (int i = 0; i < N; i++) {
	pthread_join(thread_id[i], NULL);
    }

    for (int i = 0; i < N; i++) {
	/* Destroy each forks */
	sem_destroy(&forks[i]);
    }

    pthread_mutex_destroy(&print_mutex);
    /* Destroy the waiter */
    pthread_mutex_destroy(&mutex);
}

