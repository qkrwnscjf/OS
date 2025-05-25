#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 총 Thread 개수 
#define NUM_THREAD 27

// 사용할 열과 행의 개수
#define ROW 9
#define COLUMN 9

int sudoku[ROW][COLUMN] =  {
	{6, 2, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1}, 
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{2, 8, 5, 4, 7, 3, 9, 1, 6}
};

// 각 쓰레드의 valid check가 유효한지 저장하기 위한 
// int형 list array 
// 0이면 invalid, 1이면 valid
int valid[NUM_THREAD] = {0};

typedef struct {
	int row;
	int column;
} parameters;

// 3x3 검사할 경우
void *isValid3x3(void *param) {
	int i, j;
	parameters *params = (parameters *)param;
	int row = params->row;
	int column = params->column;
	int temp;

	// valid bitmap로 통해, 숫자 검사하기
	int vmap[9] = {0};
	
	/* TODO 11: isvalid3x3의 로직을 작성
		   valid check한 인덱스 추가 */
	for (i = row; i < row + 3; i++) {
		for (j = column; j < column + 3; j++) {
		    temp = sudoku[i][j] - 1;
		    if (temp < 0 || temp > 8 || vmap[temp] == 1) { //중복 값 혹은 범위 조사
		        valid[row + column / 3] = 0;
		        goto invalid_3x3; //goto문 설정
		    }
		    vmap[temp] = 1;
		}
    }	
    	
    	valid[row + column / 3] = 1;   

	free(params);
	/* TODO 5: Exit thread */
	pthread_exit(NULL); //종료 함수
// vmap 중복된 경우, goto문을 통해 빠져 나감
invalid_3x3: 
	/* TODO 6: Exit thread */
	pthread_exit(NULL);
}

// Row 검사할 경우
void *isValidRow(void *param) {
	parameters *params = (parameters *)param;
	int row = params->row;

	// valid bitmap로 통해, 숫자 검사하기
	int vmap[9] = {0}; 
	int iter;
	int temp;

	/* TODO 12: isValidRow의 로직을 작성 
		   valid check한 인덱스 추가 */
	for(iter = 0; iter < COLUMN; iter++) {
		temp = sudoku[row][iter];
		if(temp < 1 || temp > 9 || vmap[temp -1] == 1) {// 중복 값 혹은 범위 조사
			printf("Invalid");
			free(params); 
			goto invalid_row;
		}
		vmap[temp - 1] = 1;
	}
	valid[9 + row] = 1;
	free(params);
	/* TODO 7: Exit thread */
	pthread_exit(NULL);
// vmap 중복된 경우, goto문을 통해 빠져 나감
invalid_row: 
	/* TODO 8: Exit thread */
	pthread_exit(NULL);
}

void *isValidColumn(void *param) {
	parameters *params = (parameters *)param;	
	int column = params->column;

	// valid bitmap로 통해, 숫자 검사하기
	int vmap[9] = {0};
	int temp;
	int iter;

	/* TODO 13: isValidColumn의 로직을 작성, 
		   valid check한 인덱스 추가 */
	for(iter = 0; iter < ROW; iter++) {
		temp = sudoku[iter][column];
		if(temp < 1 || temp > 9 || vmap[temp -1] == 1) { //중복 값 혹은 범위 조사
			printf("Invalid");
			free(params); 
			goto invalid_column;
		}
		vmap[temp - 1] = 1;
	}

	valid[18 + column] = 1;
	free(params);
	/* TODO 9: Exit thread */
	pthread_exit(NULL);
// vmap 중복된 경우, goto문을 통해 빠져 나감
invalid_column: 
	/* TODO 10: Exit thread */
	pthread_exit(NULL);
}

int main(void) {
	int i, j;

	// threads tid를 파악할 수 있음
	pthread_t threads[NUM_THREAD];
	int threadIndex = 0;

	for( i = 0; i < ROW; i ++ ) {

		for( j = 0; j < COLUMN; j++ ) {
			// isValid3x3
			if( i % 3 == 0 && j % 3 == 0 ) {
				parameters *data = (parameters *)malloc(sizeof(parameters));
				data->row = i;
				data->column = j;

				/* TODO 1: Create Thread */
				pthread_create(&threads[threadIndex++], NULL, isValid3x3, data); //생성 함수
				printf("%2dth thread with \'is3x3Valid\' function at \t[%d][%d]:%d\n", 
					threadIndex, i, j, sudoku[i][j] );
			}
			// isValidColumn
			if( i == 0 ) {
				parameters *data = (parameters *)malloc(sizeof(parameters));
				data->row = i; 
				data->column = j;
				
				/* TODO 2: Create Thread */
				pthread_create(&threads[threadIndex++], NULL, isValidColumn, data);
				printf("%2dth thread with \'isValidColumn\' function at \t[%d][%d]:%d\n", 
					threadIndex, i, j, sudoku[i][j] );
			}
			// isValidRow
			if( j == 0 ) {
				parameters *data = (parameters *)malloc(sizeof(parameters));
				data->row = i; 
				data->column = j;
				
				/* TODO 3: Create Thread */
				pthread_create(&threads[threadIndex++], NULL, isValidRow, data);
				printf("%2dth thread with \'isValidRow\' function at \t[%d][%d]:%d\n", 
					threadIndex, i, j, sudoku[i][j] );
			}
		}
	}

	for( i = 0; i < NUM_THREAD; i++ ) {
		/* TODO 4: wait for all created threads */
		pthread_join(threads[i], NULL);//쓰레드 종료 기다리는 함수, return 필요 없으니 NULL
		printf("%2dth thread is terminated\n", i);
	}

	for( i = 0; i < NUM_THREAD; i++ ) {
		if( valid[i] == 0 ) {
			printf("sudoku solution is invalid !!\n");
			exit(1);
		}
	}

	printf("sudoku solution is valid !! \n");

	return 0;
}
