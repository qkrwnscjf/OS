#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // pthread 관련 함수

#define DATA_SIZE 16

void mergeSort(int data[], int p, int r); // 실습 예제 참고
void merge(int data[], int p, int q, int r); // merge 실습 예제 참고

void *sort_thread(void *param); // thread sort를 위해 선언
void *merge_thread(void *param); // thread merge를 위한 선언


struct range {
	char *t_name;
	int index;
	int end;
	int *data;
};

/* TODO 7: Merge sort */
void mergeSort(int data[], int p, int r) { //실습 코드 참고
	int q;
	if( p < r ) {
		q = (p + r) / 2;
		mergeSort(data, p, q);
		mergeSort(data, q + 1, r);
		merge(data, p, q, r);
	}
}

void merge(int data[], int p, int q, int r) { // 실습 제공 코드 참고
	int i = p, j = q + 1, k = p;
	int tmp[DATA_SIZE];
	while( i  <= q && j <= r ) {
		if( data[i] <= data[j] ) 
			tmp[k++] = data[i++];
		else 
			tmp[k++] = data[j++];
	}
	while( i <= q )
		tmp[k++] = data[i++];
	while( j <= r ) 
		tmp[k++] = data[j++];
	for( int a = p; a <= r; a++ ) 
		data[a] = tmp[a];
}


void *thread_sort(void *data) // thread 정렬
{
	struct range *data_ = (struct range *)data;
	printf("%s: sort %d ~ %d\n", data_->t_name, data_->index, data_->end);
	mergeSort(data_->data, data_->index, data_->end);
	pthread_exit(0);
}

void *thread_merge(void *data) // thread merge
{
	struct range *data_ = (struct range *)data;
	printf("%s: merge %d ~ %d\n", data_->t_name, data_->index, data_->end);
	merge(data_->data, 0, data_->index, data_->end);
	pthread_exit(0);
}


int main(void) {

	int thr_id;
	pthread_t tid[3];
	int data[DATA_SIZE] = {5, 16, 4, 7, 1, 3, 2, 6, 8, 13, 11, 9, 10, 12, 15, 14};
		
	printf("<thread merge>\n");

	struct range first_half, second_half, merge_range;
	int mid = DATA_SIZE / 2;

	first_half.t_name = "thread_1";
	first_half.index = 0;
	first_half.end = mid;
	first_half.data = data;

	second_half.t_name = "thread_2";
	second_half.index = mid + 1;
	second_half.end = DATA_SIZE - 1;
	second_half.data = data;

	merge_range.t_name = "thread_3";
	merge_range.index = mid;
	merge_range.end = DATA_SIZE - 1;
	merge_range.data = data;

	/* TODO 1: 1st Thread - Sort first half of data */
	thr_id = pthread_create(&tid[0], NULL, thread_sort, &first_half);
	if( thr_id < 0 ) {
		perror("Thread create error : ");
		exit(0);
	}

	/* TODO 2: 2st Thread - Sort second half of data */
	thr_id = pthread_create(&tid[1], NULL, thread_sort, &second_half);
	if( thr_id < 0 ) {
		perror("Thread create error : ");
		exit(0);
	}

	/* TODO 4: Waits for the first thread */
	pthread_join(tid[0], NULL);

	/* TODO 5: Waits for the second thread */
	pthread_join(tid[1], NULL);

	/* TODO 3: 3rd Thread - Merge the result of two halfs */
	thr_id = pthread_create(&tid[2], NULL, thread_merge, &merge_range);
	if( thr_id < 0 ) {
		perror("Thread create error : ");
		exit(0);
	}

	/* TODO 4: Waits for the first thread */
	//pthread_join(tid[0], NULL);
	/* TODO 5: Waits for the second thread */
	//pthread_join(tid[1], NULL);
	/* TODO 6: Waits for the third thread */
	pthread_join(tid[2], NULL);

	printf("merge result\n");
	for( int i = 0; i < DATA_SIZE; i++ )
		printf("%d ", data[i]);
	printf("\n");

	return 0;
}





