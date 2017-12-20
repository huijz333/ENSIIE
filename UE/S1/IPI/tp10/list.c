# include "list.h"


/** allocate a new list on the heap */
t_list * list_new(void) {
	t_list * lst = (t_list *)malloc(sizeof(t_list));
	if (lst == NULL) {
		return (NULL);
	}
	lst->head = NULL;
	return (lst);
}


int list_isEmpty(t_list * lst) {
	return (lst->head == NULL);
}

/** delete the list from the heap */
void list_delete(t_list * lst) {
	t_node * node = lst->head;
	while (node != NULL) {
		t_node * next = node->next;
		free(node);
		node = next;
	}
	free(lst);
}

/** add an element at the end of the list */
void list_addback(t_list * lst, double v) {
	t_node * newnode = (t_node *)malloc(sizeof(t_node));
	if (newnode == NULL) {
		return ;
	}
	newnode->next = NULL;
	newnode->v = v;

	if (lst->head == NULL) {
		lst->head = newnode;
	} else {
		t_node * node = lst->head;
		while (node->next != NULL) {
			node = node->next;
		}
		node->next = newnode;
	}
}

/** add an element at the begining of the list */
void list_addfront(t_list * lst, double v) {
	t_node * newnode = (t_node *)malloc(sizeof(t_node));
	if (newnode == NULL) {
		return ;
	}
	newnode->next = lst->head;
	newnode->v = v;
	lst->head = newnode;
}

/** get the length of the list */
unsigned int list_length(t_list * lst) {
	unsigned int length = 0;
	t_node * node = lst->head;
	while (node != NULL) {
		++length;
		node = node->next;
	}
	return (length);
}

/** print the list */
void list_print(t_list * lst) {
	printf("------------------------------------------------------\n");
	printf("printing list of length %d and sum of %f\n", list_length(lst), list_sum(lst));
	t_node * node = lst->head;
	while (node != NULL) {
		printf("%lf\n", node->v);
		node = node->next;
	}
	printf("------------------------------------------------------\n");
}

/** sum elements of the list */
double list_sum(t_list * lst) {
	double sum = 0.0;
	t_node * node = lst->head;
	while (node != NULL) {
		sum += node->v;
		node = node->next;
	}
	return (sum);
}

/** return 1 if any elements matching v was found into lst */
int list_contains(t_list * lst, double v) {
	t_node * node = lst->head;
	while (node != NULL) {
		if (ABS(node->v - v) < 10e-20) {
			return (1);
		}
		node = node->next;
	}
	return (0);
}

/** return 1 if any elements matching v was removed */
int list_remove(t_list * lst, double v) {
	t_node * prev = NULL;
	t_node * node = lst->head;
	t_node * next = NULL;
	while (node != NULL) {
		next = node->next;
		if (ABS(node->v - v) < 10e-20) {
			free(node);
			if (prev == NULL) {
				lst->head = next;
			} else {
				prev->next = next;
			}
			return (1);
		}
		prev = node;
		node = next;
	}
	return (0);
}

/** return 1 if any elements matching v was removed */
int list_remove_all(t_list * lst, double v) {
	int any_removed = 0;
	t_node * prev = NULL;
	t_node * node = lst->head;
	t_node * next = NULL;
	while (node != NULL) {
		next = node->next;
		if (ABS(node->v - v) < 10e-20) {
			free(node);
			if (prev == NULL) {
				lst->head = next;
			} else {
				prev->next = next;
			}
			any_removed = 1;
		}
		prev = node;
		node = next;
	}
	return (any_removed);
}

/** remove last element from the list and return it */
double list_pop(t_list * lst) {
	if (lst->head == NULL) {
		return (-1);
	}
	t_node * node = lst->head;
	t_node * prev = NULL;
	while (node->next != NULL) {
		prev = node;
		node = node->next;
	}
	double v = node->v;
	free(node);
	if (prev == NULL) {
		lst->head = NULL;
	} else {
		prev->next = NULL;
	}
	return (v);
}

/** dupplicate the list and sort it */
t_list * list_insertsort(t_list * lst) {
	t_list * new = list_new();
	if (new == NULL) {
		return (NULL);
	}
	unsigned int l = list_length(lst);
	unsigned int n = 0;
	double prev_min = -INFINITY;
	while (n < l) {
		unsigned int count = 0;
		double min = INFINITY;
		t_node * node = lst->head;
		while (node != NULL) {
			if (node->v > prev_min && node->v < min) {
				min = node->v;
				count = 1;
			} else if (node->v == min) {
				++count;
			}
			node = node->next;
		}
		n += count;
		while (count) {
			list_addback(new, min);
			--count;
		}
		prev_min = min;
	}
	return (new);
}

/* sorts the linked list by changing next pointers (not data) */
void list_bubblesort(t_list * lst) {
	/* if zero or one node, nothing to be done */
	if (lst->head == NULL || lst->head->next == NULL) {
		return ;
	}
	
	int sorted = 0;
	while (!sorted) {
		sorted = 1;

		/* handle head case */
		if (lst->head->next->v < lst->head->v) {
			/* swap head and next */
			t_node * prevhead = lst->head;
			t_node * newhead = lst->head->next;

			prevhead->next = newhead->next;
			newhead->next = prevhead;

			lst->head = newhead;
		}
		
		/* sort rest of the list */
		t_node * jnext	= lst->head->next->next;
		t_node * j	= lst->head->next;
		t_node * jprev	= lst->head;
		while (jnext != NULL) {
			if (jnext->v < j->v) {
				/* swap j and jnext */
				jprev->next	= jnext;
				j->next		= jnext->next;
				jnext->next	= j;
				sorted = 0;
			}
			jprev = j;
			j = jnext;
			jnext = j->next;
		}
	}

}
/*
int main() {
	t_list * lst = list_new();
	list_addfront(lst, -1.0);
	list_addfront(lst, 4.0);
	list_addfront(lst, -4.0);
	list_addfront(lst, 2.0);
	list_addfront(lst, 16.0);
	list_addfront(lst, 42.0);
	list_print(lst);
	
	t_list * sorted = list_dupsort(lst);
	list_print(sorted);
	free(sorted);

	list_print(lst);
	list_sort(lst);
	list_print(lst);

	list_delete(lst);
	return (0);
}*/
