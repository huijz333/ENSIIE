#include <stdio.h>
#include "linked_list.h"

static void printList(t_list * lst) {
	LIST_ITER_START(lst, int *, integer) {
		printf("%d -> ", *integer);
	}
	LIST_ITER_END(lst, int *, integer);
	printf("NULL\n");
}

int main() {
	t_list * lst = list_new();
	int values[] = {12, 54, 6, 8, 456, 789, 32, 5, 456, 89};
	int i;
	for (i = 0 ; i < 10 ; i++) {
		list_add(lst, values + i, sizeof(int));
	}
	printList(lst);
	list_delete(lst);
	return (0);
}
