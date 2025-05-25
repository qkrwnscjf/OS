#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

int main(void) {

	int r;
	
	while(1) {
		srand(time(NULL));
		r = rand();
		if( r % 3 == 0 ) {
			goto got_my_number;
		}
	}
got_my_number:
	printf("My Favorite number : %d\n", r);
	return 0;
}
