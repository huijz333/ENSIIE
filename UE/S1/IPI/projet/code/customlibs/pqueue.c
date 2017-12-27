# include "pqueue.h"

/**
 *	Structure de donnée: 'File de priorité' , ou 'Priority Queue'
 *
 *	https://www.geeksforgeeks.org/binary-heap/
 */

/**
 *	@require: 'cmpf': fonction comparant les clefs (voir 'strcmp()')
 *	@ensure : renvoie une nouvelle file de priorité
 *	@assign : ---------------------	
 */
t_pqueue * pqueue_new(t_cmpf cmpf) {
	t_pqueue * pqueue = (t_pqueue *) malloc(sizeof(t_pqueue));
	if (pqueue == NULL) {
		return (NULL);
	}
	pqueue->nodes = array_new(16, sizeof(t_pqueue_node *));
	if (pqueue->nodes == NULL) {
		free(pqueue->nodes);
		return (NULL);
	}
	pqueue->cmpf = cmpf;
	return (pqueue);
}

/**
 *	@require: une file de priorité
 *	@ensure : supprime la file de la mémoire
 *	@assign : ---------------------
 */
void pqueue_delete(t_pqueue * pqueue) {
	ARRAY_ITERATE_START(pqueue->nodes, t_pqueue_node **, nodeRef, i) {
		free(*nodeRef);
	}
	ARRAY_ITERATE_STOP(pqueue->nodes, t_pqueue_node **, nodeRef, i);
	array_delete(pqueue->nodes);
	free(pqueue);
}

/**
 *	@require: une file de priorité
 *	@ensure : renvoie 1 si la file est vide, 0 sinon
 *	@assign : ---------------------	
 */
int pqueue_is_empty(t_pqueue * pqueue) {
	return (pqueue->nodes->size == 0);
}

/** fonction interne pour recuperer une node */
static t_pqueue_node * pqueue_get_node(t_pqueue * pqueue, unsigned int i) {
	return (*((t_pqueue_node **)array_get(pqueue->nodes, i)));
}

/** fonction interne pour echanger deux sommets de la file */
static void pqueue_swap_nodes(t_pqueue * pqueue, unsigned int i, unsigned int j) {
	t_pqueue_node * inode = pqueue_get_node(pqueue, i);
	t_pqueue_node * jnode = pqueue_get_node(pqueue, j);
	array_set(pqueue->nodes, i, &jnode);
	array_set(pqueue->nodes, j, &inode);
	inode->index = j;
	jnode->index = i;
}

/**
 *	@require: une file de priorité, une clef, et une valeur
 *			renvoie 1 si l'élément a pu etre inseré, 0 sinon
 *	@ensure : ajoutes l'élément dans la file.
 *			- il y a copie des adresses, pas des valeurs!
 *		  	- si la clef doit être modifié, appelé 'pqueue_decrease()'
 *	@assign : --------------------------
 */
t_pqueue_node * pqueue_insert(t_pqueue * pqueue, void const * key, void const * value) {
	// First insert the new key at the end
	t_pqueue_node * node = (t_pqueue_node *) malloc(sizeof(t_pqueue_node));
	if (node == NULL) {
		return (NULL);
	}
	/** initialise le sommet de la file */
	node->key	= key;
	node->value	= value;
	node->index	= array_add(pqueue->nodes, &node);

	// Fix the min heap property if it is violated
	unsigned int i = pqueue->nodes->size - 1;
	while (i != 0) {
		/** parent of 'i' */
		unsigned int pi = (i - 1) / 2;
		t_pqueue_node * i_node	= pqueue_get_node(pqueue, i);
		t_pqueue_node * pi_node	= pqueue_get_node(pqueue, pi);
		if (pqueue->cmpf(i_node->key, pi_node->key) > 0) {
			i = pi;
			continue ;
		}
		pqueue_swap_nodes(pqueue, i, pi);
		i = pi;
	}
	return (node);
}

/**
 *	@require: une file de priorité, un sommet, et une nouvelle clef plus faible
 *		  que la precedente pour ce sommet
 *	@ensure : diminue la clef d'une valeur (<=> augmente sa priorité)
 *	@assign : ----------------------
 */
void pqueue_decrease(t_pqueue * pqueue, t_pqueue_node * node, void const * newkey) {
	node->key = newkey;
	unsigned int i = node->index;
	while (i != 0) {
		unsigned int pi = (i - 1) / 2;
		t_pqueue_node * i_node	= pqueue_get_node(pqueue, i);
		t_pqueue_node * pi_node	= pqueue_get_node(pqueue, pi);
		if (pqueue->cmpf(pi_node->key, i_node->key) < 0) {
			break ;
		}
		pqueue_swap_nodes(pqueue, i, pi);
		i = pi;
	}
}

/**
 *	@require: une file de priorité
 *	@ensure: renvoie l'élément avec la clef la plus basse (<=> la plus haute priorité)
 *	@assign: --------------------------
 */
t_pqueue_node * pqueue_minimum(t_pqueue * pqueue) {
	return (pqueue_get_node(pqueue, 0));
}

/**
 *	fonction interne qui assure l'intégrité de la structure de manière recursive,
 *  	en partant de l'index 'i'
 */
static void pqueue_heapify(t_pqueue * pqueue, unsigned int i) {
	t_array * nodes = pqueue->nodes;
	unsigned int l = 2 * i + 1;
	unsigned int r = 2 * i + 2;
	unsigned int s = i; /** plus petite clef entre 'i', 'l', et 'r' */
	t_pqueue_node * inode = pqueue_get_node(pqueue, i);

	if (l < nodes->size) {
		t_pqueue_node * lnode = pqueue_get_node(pqueue, l);
		if (pqueue->cmpf(lnode->key, inode->key) < 0) {
			s = l;
		}
	}

	if (r < nodes->size) {
		t_pqueue_node * rnode = pqueue_get_node(pqueue, r);
		t_pqueue_node * snode = pqueue_get_node(pqueue, s);
		if (pqueue->cmpf(rnode->key, snode->key) < 0) {
			s = r;
		}
	}

	if (s != i) {
		pqueue_swap_nodes(pqueue, i, s);
		pqueue_heapify(pqueue, s);
	}
}

/**
 *	@require: une file de priorité
 *	@ensure: supprime et renvoie l'élément de la file avec
 *		 la clef la plus basse (<=> la plus haute priorité)
 *	@assign: --------------------------
 */
t_pqueue_node pqueue_pop(t_pqueue * pqueue) {
	if (pqueue->nodes->size == 0) {
		t_pqueue_node node;
		node.key = NULL;
		node.value = NULL;
		return (node);
	}

	/** sauvegarde le minimum */
	t_pqueue_node * min = pqueue_minimum(pqueue);

	t_array * nodes = pqueue->nodes;
	if (nodes->size == 1) {
		array_remove(nodes, 0);
	} else {
		pqueue_swap_nodes(pqueue, 0, nodes->size - 1);
		array_removelast(nodes);
		pqueue_heapify(pqueue, 0);
	}

	/** renvoie une copie sur la stack */
	t_pqueue_node minCopy = *min;
	free(min);
	return (minCopy);
}

static int dblcmp(double * a, double * b) {
	if (*a < *b) {
		return (-1);
	}
	if (*a > *b) {
		return (1);
	}
	return (0);
}

/*
	TESTS et exemple d'utilisation
*/
/*
int main() {

	t_pqueue * pqueue = pqueue_new((t_cmpf)dblcmp);

	double zero = 0;
	double one = 1;
	double two = 2;

	pqueue_insert(pqueue, &one, &one);
	pqueue_insert(pqueue, &zero, &one);
	pqueue_insert(pqueue, &two, &one);

	t_pqueue_node node = pqueue_pop(pqueue);
	double v = *((double *)node.key);
	printf("%lf\n", v);

	pqueue_delete(pqueue);
}
*/
/*
#include <stdlib.h>
#include <time.h>
int main() {
	t_pqueue * pqueue = pqueue_new((t_cmpf)cmpint);

	srand(time(NULL));
	rand();

	int n = 4096;
	t_pqueue_node ** nodes = (t_pqueue_node **)malloc(sizeof(t_pqueue_node *) * n);
	int i;
	for (i = 0 ; i < n ; i++) {
		int * key = (int *)malloc(sizeof(int));
		int * value = (int *) malloc(sizeof(int));
		*key = rand();
		*value = rand();
		nodes[i] = pqueue_insert(pqueue, key, value);
	}

	for (i = 0 ; i < n ; i++) {
		t_pqueue_node * node = nodes[i];
		int * oldkeyRef = (int *) node->key;
		int newkey = rand();
		if (newkey < *oldkeyRef) {
			int * newkeyRef = (int*) malloc(sizeof(int));
			*newkeyRef = newkey;
			pqueue_decrease(pqueue, node, newkeyRef);
			free(oldkeyRef);
		}
	}

	for (i = 0 ; i < n ; i++) {
		t_pqueue_node node = pqueue_pop(pqueue);
		int * key = (int *)node.key;
		int * value = (int *)node.value;
		printf("%d\n", *key);
		free(key);
		free(value);
	}
	free(nodes);
	pqueue_delete(pqueue);
	return (0);
}*/
