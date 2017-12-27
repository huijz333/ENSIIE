# include "pqueue_pairingheap.h"

/**
 *	Structure de donnée: 'File de priorité' , ou 'Priority Queue'
 *
 *	Très peu documenté en Français... voir la page wikipédia anglaise
 *	qui est vraiment complète
 *	
 *	https://en.wikipedia.org/wiki/Pairing_heap
 *	https://users.cs.fiu.edu/~weiss/dsaajava2/code/PairingHeap.java
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
	pqueue->cmpf = cmpf;
	pqueue->size = 0;
	pqueue->root = NULL;
	return (pqueue);
}

/**
 *	@require: une file de priorité
 *	@ensure : supprime la file de la mémoire
 *	@assign : ---------------------
 */
void pqueue_delete(t_pqueue * pqueue) {
	puts("not fully implemented yet!");
	free(pqueue);
}

/**
 *	@require: une file de priorité
 *	@ensure : renvoie 1 si la file est vide, 0 sinon
 *	@assign : ---------------------	
 */
int pqueue_is_empty(t_pqueue * pqueue) {
	return (pqueue->size == 0);
}

/**
 *	fonction interne pour garder l'ordre dans la file
 */
static t_pqueue_node * pqueue_node_link(t_pqueue_node * first,
					t_pqueue_node * second,
					t_cmpf cmpf) {
	if (second == NULL) {
		return (first);
	}

	if (cmpf(second->key, first->key) < 0) {
		// Attach first as leftmost child of second
		second->prev = first->prev;
		first->prev = second;
		first->nextSibling = second->leftChild;
		if (first->nextSibling != NULL) {
			first->nextSibling->prev = first;
		}
		second->leftChild = first;
		return (second);
	} else {
		// Attach second as leftmost child of first
		second->prev = first;
		first->nextSibling = second->nextSibling;
		if (first->nextSibling != NULL) {
			first->nextSibling->prev = first;
		}
		second->nextSibling = first->leftChild;
		if (second->nextSibling != NULL) {
			second->nextSibling->prev = second;
		}
		first->leftChild = second;
		return (first);
	}

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
	t_pqueue_node * newnode = (t_pqueue_node *) malloc(sizeof(t_pqueue_node));
	if (newnode == NULL) {
		return (NULL);
	}
	newnode->key = key;
	newnode->value = value;
	newnode->leftChild = NULL;
	newnode->prev = NULL;
	newnode->nextSibling = NULL;

	if (pqueue->root == NULL) {
		pqueue->root = newnode;
	} else {
		pqueue->root = pqueue_node_link(pqueue->root, newnode, pqueue->cmpf);
	}

	++pqueue->size;
	return (newnode);
}

/**
 *	@require: une file de priorité, et le sommet de la file
 *	@ensure : diminue la clef d'une valeur (<=> augmente sa priorité)
 *	@assign : ----------------------
 */
void pqueue_decrease(t_pqueue * pqueue, t_pqueue_node * node, void const * newKey);

/**
 *	@require: une file de priorité
 *	@ensure: renvoie l'élément avec la clef la plus basse (<=> la plus haute priorité)
 *	@assign: --------------------------
 */
t_pqueue_node * pqueue_minimum(t_pqueue * pqueue) {
	return (pqueue->root);
}


    /**
           * Internal method that implements two-pass merging.
	        * @param firstSibling the root of the conglomerate;
		     *     assumed not NULL.
		          */
t_pqueue_node * combineSiblings(t_pqueue_node * firstSibling, t_cmpf cmpf) {
	if (firstSibling->nextSibling == NULL) {
		return (firstSibling);
	}

	t_array * treeArray = array_new(16, sizeof(t_pqueue_node *));

	// Store the subtrees in an array
	while (firstSibling != NULL) {
		array_add(treeArray, &firstSibling);
		firstSibling->prev->nextSibling = NULL;  // break links
		firstSibling = firstSibling->nextSibling;
	}

	// Combine subtrees two at a time, going left to right
	size_t i = 0;
	while (i + 1 < treeArray->size) {
		t_pqueue_node * first	= *((t_pqueue_node **) array_get(treeArray, i));
		t_pqueue_node * second	= *((t_pqueue_node **) array_get(treeArray, i));
		t_pqueue_node * head	= pqueue_node_link(first, second, cmpf);
		array_set(treeArray, i, &head);
		i += 2;
	}

	// j has the result of last pqueue_node_link.
	// If an odd number of trees, get the last one.
	size_t j = i - 2;
	if (j == treeArray->size - 3) {
		t_pqueue_node * first	= *((t_pqueue_node **) array_get(treeArray, j));
		t_pqueue_node * second	= *((t_pqueue_node **) array_get(treeArray, j + 2));
		t_pqueue_node * head  = pqueue_node_link(first, second, cmpf);
		array_set(treeArray, j, &head);
	}

	// Now go right to left, merging last tree with
	// next to last. The result becomes the new last.
	while (j >= 2) {
		t_pqueue_node * first	= *((t_pqueue_node **) array_get(treeArray, j - 2));
		t_pqueue_node * second	= *((t_pqueue_node **) array_get(treeArray, j));
		t_pqueue_node * head  = pqueue_node_link(first, second, cmpf);
		array_set(treeArray, j - 2, &head);
		j -= 2;
	}

	t_pqueue_node * head = *((t_pqueue_node **) array_get(treeArray, 0));
	array_delete(treeArray);
	return (head);
}

/**
 *	@require: une file de priorité
 *	@ensure: supprime et renvoie l'élément de la file avec
 *		 la clef la plus basse (<=> la plus haute priorité)
 *	@assign: --------------------------
 */
t_pqueue_node * pqueue_pop(t_pqueue * pqueue) {
	if (pqueue_is_empty(pqueue)) {
		return (NULL);
	}

	t_pqueue_node * min = pqueue->root;
	if (min->leftChild == NULL) {
		pqueue->root = NULL;
	} else {
		pqueue->root = combineSiblings(pqueue->root->leftChild, pqueue->cmpf);
	}

	--pqueue->size;
	return (min);
}

static int cmpint(int * a, int * b) {
	return (*a - *b);
}

/*
	TESTS et exemple d'utilisation

#include <stdlib.h>
#include <time.h>
int main() {
	t_pqueue * pqueue = pqueue_new((t_cmpf)cmpint);

	srand(time(NULL));
	rand();

	int n = 4096;
	t_pqueue_node ** nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node*) * n);
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
		t_pqueue_node * node = pqueue_pop(pqueue);
		int * key = (int *)node->key;
		int * value = (int *)node->value;
		printf("%d\n", *key);
		free(key);
		free(value);
		pqueue_node_delete(node);
	}
	free(nodes);
	pqueue_delete(pqueue);
	return (0);
}*/
/*
int main() {
	t_pqueue * pqueue = pqueue_new((t_cmpf)cmpint);

	int * key = (int *)malloc(sizeof(int));
	int * value = (int *) malloc(sizeof(int));
	*key = 1;
	*value = 42;
	pqueue_insert(pqueue, key, value);
	t_pqueue_node * min = pqueue_pop(pqueue);
	printf("%d : %d\n", *((int*)min->key), *((int*)min->value));
	free((void*)min->key);
	free((void*)min->value);
	free(min);
	
	
	key = (int *)malloc(sizeof(int));
	value = (int *) malloc(sizeof(int));
	*key = 2;
	*value = 43;
	pqueue_insert(pqueue, key, value);
	min = pqueue_pop(pqueue);
	printf("%d : %d\n", *((int*)min->key), *((int*)min->value));
	free((void*)min->key);
	free((void*)min->value);
	free(min);

	int n = 40;
	int i;
	for (i = 0 ; i < n ; i++) {
		key = (int *)malloc(sizeof(int));
		value = (int *) malloc(sizeof(int));
		*key = 3 + i;
		*value = 44 + i;
		pqueue_insert(pqueue, key, value);
	}

	while (!pqueue_is_empty(pqueue)) {
		min = pqueue_pop(pqueue);
		printf("%d : %d\n", *((int*)min->key), *((int*)min->value));
		free((void*)min->key);
		free((void*)min->value);
		free(min);
	}
		
	
	
	
	pqueue_delete(pqueue);
	
	
	
	return (0);
}*/
