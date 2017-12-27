#ifndef PQUEUE_FIBONACCI_H
# define PQUEUE_FIBONACCI_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/**
 *	Structure de donnée: 'File de priorité' , ou 'Priority Queue'
 *
 *	Très peu documenté en Français... voir la page wikipédia anglaise
 *	qui est vraiment complète
 *
 *	https://en.wikipedia.org/wiki/Priority_queue
 *
 *	Implémentation basé sur le travail de:
 *
 *	https://github.com/mikepound/mazesolving/blob/master/FibonacciHeap.py
 */

# ifndef BYTE
#	define BYTE unsigned char
# endif
# ifndef BIT
#	define BIT BYTE
# endif

/** typedef pratiqe pour les fonctions de comparaisons */
typedef int (*t_cmpf)(void const * leftKey, void const * rightKey);

/** sommet (== feuilles ou racine) de la liste */
typedef struct	s_pqueue_node {
	void const		* key;
	void const 		* value;
	size_t			degree;
	BIT			flag;
	struct s_pqueue_node	* next;
	struct s_pqueue_node	* prev;
	struct s_pqueue_node	* child;
	struct s_pqueue_node	* parent;
}		t_pqueue_node;

/**
 *	@require : l'adresse d'une clef, l'adresse d'une valeur
 *	@ensure  : renvoie un nouveau sommet contenant la clef et la valeur
 *			- les données ne sont pas copiés, seules les adresses le sont.
 *	@assign  : initialise le sommet pour l'insérer dans la file
 */
t_pqueue_node * pqueue_node_new(void const * key, void const * value);

/**
 *
 *	@require : un sommet
 *	@ensure  : supprime le sommet de la mémoire
 *			- ATTENTION : si ce sommet est encore dans la file,
 *			le supprimer de la file avant
 *	@assign  : --------
 */
void pqueue_node_delete(t_pqueue_node * node);


/** la file de priorité */
typedef struct	s_pqueue {
	t_pqueue_node	* min;
	t_cmpf		cmpf;
	size_t		size;
	size_t		maxdegree;
}		t_pqueue;

/**
 *	@require: 'cmpf': fonction comparant les clefs (voir 'strcmp()')
 *	@ensure : renvoie une nouvelle file de priorité
 *	@assign : ---------------------	
 */
t_pqueue * pqueue_new(t_cmpf cmpf);

/**
 *	@require: une file de priorité
 *	@ensure : supprime la file de la mémoire
 *	@assign : ---------------------
 */
void pqueue_delete(t_pqueue * pqueue);

/**
 *	@require: une file de priorité
 *	@ensure : renvoie 1 si la file est vide, 0 sinon
 *	@assign : ---------------------	
 */
int pqueue_is_empty(t_pqueue * pqueue);

/**
 *	@require: une file de priorité, une clef, et une valeur
 *			renvoie 1 si l'élément a pu etre inseré, 0 sinon
 *	@ensure : ajoutes l'élément dans la file.
 *			- il y a copie des adresses, pas des valeurs!
 *		  	- si la clef doit être modifié, appelé 'pqueue_decrease()'
 *	@assign : --------------------------
 */
t_pqueue_node * pqueue_insert(t_pqueue * pqueue, void const * key, void const * value);

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
t_pqueue_node * pqueue_minimum(t_pqueue * pqueue);

/**
 *	@require: une file de priorité
 *	@ensure: supprime et renvoie l'élément de la file avec
 *		 la clef la plus basse (<=> la plus haute priorité)
 *	@assign: --------------------------
 */
t_pqueue_node * pqueue_pop(t_pqueue * pqueue);

#endif
