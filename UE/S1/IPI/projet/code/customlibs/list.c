# include "list.h"

/**
 *	@require: ---------------------
 *	@ensure : renvoie une nouvelle liste doublement chainée
 *	@assign : ---------------------
 */
t_list * list_new(void) {
	t_list * lst = (t_list *) malloc(sizeof(t_list));
	if (lst == NULL) {
		return (NULL);
	}
	lst->head = (t_list_node*)malloc(sizeof(t_list_node));
	if (lst->head == NULL) {
		free(lst);
		return (NULL);
	}
	lst->head->next = lst->head;
	lst->head->prev = lst->head;
	lst->size = 0;
	return (lst);

}

/**
 *	@require: une liste, une donnée, et la taille de la donnée
 *	@ensure : ajoute une copie de la donnée en debut de liste
 *	@assign : ---------------------
 */
void * list_add(t_list * lst, void const * content, unsigned int content_size) {
	t_list_node * node = (t_list_node *) malloc(sizeof(t_list_node) + content_size);
	if (node == NULL) {
		return (NULL);
	}
	memcpy(node + 1, content, content_size);

	t_list_node * tmp = lst->head->prev;

	lst->head->prev = node;
	tmp->next = node;

	node->prev = tmp;
	node->next = lst->head;

	++lst->size;

	return (node + 1);

}

/**
 *	@require: une liste, une donnée, et la taille de la donnée
 *	@ensure : ajoute une copie de la donnée en fin de liste
 *	@assign : ---------------------
 */
void * list_push(t_list * lst, void const * content, unsigned int content_size) {
	t_list_node * node = (t_list_node *) malloc(sizeof(t_list_node) + content_size);
	if (node == NULL) {
		return (NULL);
	}
	memcpy(node + 1, content, content_size);

	/** head <-> tmp <-> ... */
	t_list_node * tmp = lst->head->next;

	/** head -> node <- tmp <-> ... */
	lst->head->next = node;
	tmp->prev = node;

	/** head <-> node <-> tmp <-> ... */
	node->prev = lst->head;
	node->next = tmp;

	++lst->size;

	return (node + 1);
}

/**
 *	@require: une liste
 *	@ensure : renvoie la donnée en tête de liste 
 *	@assign : ---------------------
 */
void * list_head(t_list * lst) {
	if (lst->size == 0) {
		return (NULL);
	}
	return ((void *) (lst->head->next + 1));
}

/**
 *	@require : un sommet de la liste
 *	@ensure  : renvoie la donnée stockée par ce sommet
 *	@assign  : ------------
 */
void * list_node_data(t_list_node * node) {
	return (node + 1);
}

/**
 *	@require: une liste
 *	@ensure : renvoie la donnée en fin de liste 
 *	@assign : ---------------------
 */
void * list_tail(t_list * lst) {
	if (lst->size == 0) {
		return (NULL);
	}
	return ((void *) (lst->head->prev + 1));
}

/**
 *	Fonction interne pour supprimé un sommet de la liste
 *
 *	@require: une liste, un sommet
 *	@ensure : supprime le sommet de la liste
 *	@assign : ---------------------
 */
void list_remove_node(t_list * lst, t_list_node * node) {
	if (node->prev != NULL) {
		node->prev->next = node->next;
	}
	if (node->next != NULL) {
		node->next->prev = node->prev;
	}

	node->next = NULL;
	node->prev = NULL;
	free(node);
	lst->size--;
}

/**
 *	@require: une liste
 *	@ensure : supprime la donnée en tête de liste
 *			renvoie 1 si une donnée a été supprimé, 0 sinon
 *	@assign : ---------------------
 */
int list_remove_head(t_list * lst) {
	if (lst->size == 0) {
		return (0);
	}
	list_remove_node(lst, lst->head->next);
	return (1);
}

/**
 *	@require: une liste
 *	@ensure : supprime la donnée en fin de liste
 *			renvoie 1 si une donnée a été supprimé, 0 sinon
 *	@assign : ---------------------
 */
int list_remove_tail(t_list * lst) {
	if (lst->size == 0) {
		return (0);
	}
	list_remove_node(lst, lst->head->prev);
	return (1);
}

/**
 *	@require: une liste, une fonction
 *	@ensure : applique la fonction à toutes les valeurs de la liste
 *	@assign : ---------------------
 */
void list_iterate(t_list * lst, void (*f)(void const * value)) {
	t_list_node * node = lst->head->next;
	while (node != lst->head) {
		f(node + 1);
		node = node->next;
	}
}

/**
 *	@require: une liste
 *	@ensure : supprime toutes les données de la liste
 *	@assign : ---------------------
 */
void list_clear(t_list * lst) {
	t_list_node * node = lst->head->next;
	while (node != lst->head) {
		t_list_node * next = node->next;
		free(node);
		node = next;
	}
	lst->size = 0;
}

/**
 *	@require: une liste
 *	@ensure : supprime la liste de la mémoire
 *	@assign : ---------------------
 */
void list_delete(t_list * lst) {
	if (lst == NULL) {
		return ;
	}
	if (lst->size > 0) {
		list_clear(lst);
	}
	free(lst->head);
	free(lst);
}
