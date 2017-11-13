#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"



int main() {
	t_list * lst = list_new();

	list_delete(lst);
	return (0);
}
