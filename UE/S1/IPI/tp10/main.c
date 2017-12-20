#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	t_list * l = list_new();
	int i;
	
	/*srand(time(NULL));*/
	srand(42);
	for (i = 0; i < 100000; i++) {
		list_addfront(l, rand()%1000000 / (double)(rand()%1000000+1));
	}
	
	list_sort(l);
	list_print(l);
	list_delete(l);
	return EXIT_SUCCESS;
}
