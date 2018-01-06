# include "pqueue_fibonacci.h"

/**
 *	Structure de donnée: 'File de priorité' , ou 'Priority Queue'
 *
 *	Très peu documenté en Français... voir la page wikipédia anglaise
 *	qui est vraiment complète
 *
 *	https://en.wikipedia.org/wiki/Priority_queue
 */

/**
 *	@require : l'adresse d'une clef, l'adresse d'une valeur
 *	@ensure  : renvoie un nouveau sommet contenant la clef et la valeur
 *			- les données ne sont pas copiés, seules les adresses le sont.
 *	@assign  : initialise le sommet pour l'insérer dans la file
 */
t_pqueue_node * pqueue_node_new(void const * key, void const * value) {
	t_pqueue_node * node = (t_pqueue_node *) malloc(sizeof(t_pqueue_node));
	if (node == NULL) {
		return (NULL);
	}
	
	node->key = key;
	node->value = value;
	node->degree = 0;
	node->flag = 0;

	node->prev	= node;
	node->next	= node;
	node->parent 	= NULL;
	node->child	= NULL;
	return (node);
}

/**
 *
 *	@require : un sommet
 *	@ensure	 : supprime le sommet de la mémoire
 *			- ATTENTION : si ce sommet est encore dans la file,
 *			peut causer des erreurs!
 *	@assign  : --------
 */
void pqueue_node_delete(t_pqueue_node * node) {
	free(node);
}

/** fonction interne pour insérer un sommet dans son arbre */
static void pqueue_node_insert(t_pqueue_node * parent, t_pqueue_node * node) {
	if (node == NULL) {
		return ;
	}

	parent->next->prev = node->prev;
	node->prev->next = parent->next;
	parent->next = node;
	node->prev = parent;
}

/** fonction interne pour supprime un sommet de son arbre */
static void pqueue_node_remove(t_pqueue_node * node) {
	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->next	= node;
	node->prev	= node;
}

/** fonction interne pour ajouter un sommet à son arbre */
static void pqueue_node_addchild(t_pqueue_node * parent, t_pqueue_node * child) {
	if (parent->child == NULL) {
		parent->child = child;
	} else {
		pqueue_node_insert(parent->child, child);
	}
	child->parent = parent;
	child->flag = 0;
	++parent->degree;
}

/** fonction interne pour savoir si une feuille est seule dans son arbre */
static int pqueue_node_issingle(t_pqueue_node * node) {
	return (node->next == node);
}

/** fonction interne pour supprime un sommet */
static void pqueue_node_removechild(t_pqueue_node * parent, t_pqueue_node * node) {
	if (pqueue_node_issingle(node)) {
		parent->child = NULL;
	} else {
		if (parent->child == node) {
			parent->child = node->next;
		}
		pqueue_node_remove(node);
	}
	node->parent = NULL;
	node->flag = 0;
	parent->degree -= 1;
}

/**
 *	@require	'cmpf': fonction comparant les clefs (voir 'strcmp()')
 *	@ensure : renvoie une nouvelle file de priorité
 *	@assign : ---------------------	
 */
t_pqueue * pqueue_new(t_cmpf cmpf) {
	t_pqueue * pqueue = (t_pqueue *) malloc(sizeof(t_pqueue));
	if (pqueue == NULL) {
		return (NULL);
	}
	pqueue->min = NULL;
	pqueue->cmpf = cmpf;
	pqueue->size = 0;
	pqueue->maxdegree = 0;

	return (pqueue);
}

/**
 *	@require: une file de priorité
 *	@ensure : supprime la file de la mémoire
 *	@assign : ---------------------
 */
void pqueue_delete(t_pqueue * pqueue) {
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

/** fonction interne pour insérer un sommet dans la file */
static void pqueue_insert_node(t_pqueue * pqueue, t_pqueue_node * node) {
	if (pqueue->min == NULL) {
		pqueue->min = node;
	} else {
		pqueue_node_insert(pqueue->min, node);
		if (pqueue->cmpf(node->key, pqueue->min->key) < 0) {
			pqueue->min = node;
		}
	}
}

/**
 *	@require: une file de priorité, une clef, et une valeur
 *			renvoie le sommet insérer, ou NULL si erreur
 *	@ensure : ajoutes l'élément dans la file.
 *		  si la clef doit être modifié, appelé 'pqueue_decrease()'
 *	@assign : --------------------------
 */
t_pqueue_node * pqueue_insert(t_pqueue * pqueue, void const * key, void const * value) {
	t_pqueue_node * node = pqueue_node_new(key, value);
	if (node == NULL) {
		return (0);
	}
	pqueue_insert_node(pqueue, node);
	++pqueue->size;
	return (node);
}

/**
 *	@require: une file de priorité, une clef, et une valeur
 *	@ensure : modifie la clef d'une valeur (baisse de clef <=> augmente sa priorité)
 *	@assign : ----------------------
 */
void pqueue_decrease(t_pqueue * pqueue, t_pqueue_node * node, void const * newkey) {
	int diff = pqueue->cmpf(newkey, node->key);
	if (diff > 0) {
		puts("Tried to increase a priority! only decrease allowed");
		return ;
	}
	/** si les clefs sont identiques... pas besoin de modifier les arbres */
	if (diff == 0 && newkey != node->key) {
		node->key = newkey;
		return ;
	}
	
	node->key = newkey;
	t_pqueue_node * parent = node->parent;
	/** si l'on a mis a jour une racine d'un des arbres */
	if (parent == NULL) {
		/** on vérifie voir si cette racine devient le nouveau minimum */
		if (pqueue->cmpf(newkey, pqueue->min->key) < 0) {
			pqueue->min = node;
		}
		return ;
	}
	
	/** si on a mis à jour une feuille, mais que cette modification
	  n'a pas changé la hierarchie de priorité, on s'arrête */
	if (pqueue->cmpf(parent->key, newkey) <= 0) {
		return ;
	}

	/** sinon, on remonte la feuille */
	while (1) {
		pqueue_node_removechild(parent, node);
		pqueue_insert_node(pqueue, node);
		if (parent->parent == NULL) {
			break ;
		}

		if (parent->parent == NULL) {
			break ;
		}
		
		if (!parent->flag) {
			parent->flag = 1;
			break ;
		} 
		node = parent;
		parent = parent->parent;
	}
}

/**
 *	@require: une file de priorité
 *	@ensure: renvoie l'élément avec la clef la plus basse (<=> la plus haute priorité)
 *	@assign: --------------------------
 */
t_pqueue_node * pqueue_minimum(t_pqueue * pqueue) {
	return (pqueue->min);
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
	t_pqueue_node * removed_node = pqueue->min;
	--pqueue->size;

	/* Assign all old root children as new roots */
	if (pqueue->min->child != NULL) {
		t_pqueue_node * c = pqueue->min->child;
		while (1) {
			c->parent = NULL;
			c = c->next;
			if (c == pqueue->min->child) {
				break ;
			}
		}
		pqueue->min->child = NULL;
		pqueue_node_insert(pqueue->min, c);
	}

	/* 2.1: If we have removed the last key */
	if (pqueue->min->next == pqueue->min) {
		if (pqueue->size != 0) {
			puts("Error pqueue_pop(1)! pointers were lost as"\
					"pqueue->size != 0 and pqueue->min == 0");
		}
		pqueue->min = NULL;
		return (removed_node);
	}

	/* 2.2: Merge any roots with the same degree */
	size_t logsize = pqueue->maxdegree + 1;
	size_t memsize = logsize * sizeof(t_pqueue_node *);
	t_pqueue_node ** degreeroots = (t_pqueue_node **) malloc(memsize);
	memset(degreeroots, 0, memsize);

	pqueue->maxdegree = 0;
	t_pqueue_node * currentpointer = pqueue->min->next;

	while (1) {
		size_t currentdegree = currentpointer->degree % logsize;
		t_pqueue_node * current = currentpointer;
		currentpointer = currentpointer->next;
		while (degreeroots[currentdegree] != NULL) {

			t_pqueue_node * other = degreeroots[currentdegree];
			/* Swap if required */
			if (pqueue->cmpf(current->key, other->key) > 0) {
				t_pqueue_node * tmp = other;
				other = current;
				current = tmp;
			}
			pqueue_node_remove(other);
			pqueue_node_addchild(current, other);
			degreeroots[currentdegree] = NULL;
			++currentdegree;
		}

		degreeroots[currentdegree] = current;
		if (currentpointer == pqueue->min) {
			break;
		}
	}

	/* 3: Remove current root and find new->min */
	pqueue->min = NULL;
	size_t newmaxdegree = 0;
	size_t d;
	for (d = 0 ; d < logsize ; d++) {
		if (degreeroots[d] != NULL) {
			degreeroots[d]->next = degreeroots[d];
			degreeroots[d]->prev = degreeroots[d];
			pqueue_insert_node(pqueue, degreeroots[d]);
			if (d > newmaxdegree) {
				newmaxdegree = d;
			}
		}
	}
	free(degreeroots);

	pqueue->maxdegree = newmaxdegree;
	return (removed_node);
}

int cmpint(int * left, int * right) {
	return (*left - *right);
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
