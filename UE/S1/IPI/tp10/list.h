#ifndef LIST_H
# define LIST_H

# include <stdio.h> /*printf*/
# include <stdlib.h> /*malloc, free*/
# include <math.h> /*NAN*/
# include <float.h>
# ifndef NAN
#	define NAN (-1)
# endif

# ifndef INFINITY
#	define INFINITY (DBL_MAX)
# endif

# ifndef ABS
#	define ABS(V) ((V) < 0 ? -(V) : (V))
# endif

typedef struct	s_node {
	double v;
	struct s_node * next;
}		t_node;

typedef struct s_list {
	struct s_node * head;
}		t_list;



t_list * list_new(void);
int list_isEmpty(t_list * lst);
void list_delete(t_list * lst);
void list_addback(t_list * lst, double v);
void list_addfront(t_list * lst, double v);
void list_print(t_list * lst);
double list_sum(t_list * lst);
unsigned int list_length(t_list * lst);
int list_contains(t_list * lst, double v);
int list_remove(t_list * lst, double v);
int list_remove_all(t_list * lst, double v);
double list_pop(t_list * lst);
t_list * list_insertsort(t_list * lst);
void list_bubblesort(t_list * lst);
void list_sort(t_list * lst);

#endif
