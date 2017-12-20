# include "list.h"

/** insertion sort the list */
void list_sort(t_list * lst) {
	lst->head = list_insertsort(lst)->head;
}
