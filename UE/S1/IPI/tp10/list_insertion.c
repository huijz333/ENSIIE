# include "list.h"

static void min_to_head(t_node ** headRef) {
	t_node * head = *headRef;
	if (head == NULL || head->next == NULL) {
		return ;
	}
	t_node * iprev = head;
	t_node * i = head->next;
	t_node * min = head;
	t_node * prevmin = NULL;
	while (i != NULL) {
		if (i->v < min->v) {
			prevmin = iprev;
			min = i;
		}
		iprev = i;
		i = i->next;
	}
	if (min != head) {
		t_node * prevhead = *headRef;
		*headRef = min;
		prevmin->next = min->next;
		min->next = prevhead;
	}
}

/** insertion sort the list */
void list_sort(t_list * lst) {
	t_node ** headRef = &(lst->head);
	while (*headRef != NULL) {
		min_to_head(headRef);
		headRef = &((*headRef)->next);
	}
	list_print(lst);

}
