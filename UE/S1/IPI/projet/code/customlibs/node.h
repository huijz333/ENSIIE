#ifndef NODE_H
# define NODE_H

# include "ipi.h"
# include "array.h"

/**
 *	Represente un sommet du graphe (exo 1)
 */
typedef struct	s_node {
	INDEX	pathlen;/* nombre de sommets entre s et ce sommet */
	INDEX	prev;	/* element precedant le chemin (index du sommet) */
	BIT	visited;/* vrai ou faux selon que le sommet a été visité */
}		t_node;

/**
 *	Represente un sommet du graphe, avec un chemin pondéré (exo 2)
 */
typedef struct	s_nodew {
	t_node	super;	/* héritage 'à la C' */	
	size_t	pathw;	/* le poids total du chemin */
}		t_nodew;

/**
 *	Represente un sommet du graphe du labyrinthe (exo 3)
 */
typedef struct	s_nodel {
	t_node	super;	/* héritage 'à la C' */
	INDEX	x;
	INDEX	y;
}		t_nodel;

/**
 *	@require : 'path'  : un tableau renvoyé par "node_build_path()"
 *		   'stream': un flux
 *	@ensure  : affiche le chemin du tableau sur le flux
 *	@assign  : ---------------------------------
 */
void node_write_path(t_array * path, FILE * stream);	

/**
 *	@require : 'nodes : les sommets résolus
 *		   's'    : l'indice source
 *		   't'    : l'indice de destination
 *	@ensure  : renvoie un tableau qui contient le chemin allant de 's' à 't'
 *	@assign  : ---------------------------------
 */
t_array * node_build_path(t_array * nodes, INDEX s, INDEX t);

/**
 *	@require : 	'nodes' : sommets resolu par un algorithme du plus court chemin
 *		   	's'	: sommet source de la resolution
 *			't'	: sommet vers lequel on construit le chemin
 *			'stream': le flux
 *	@ensure	 : génère et affiche le chemin sur le flux donnée
 *		   à partir des sommets resolus par un algorithme
 *	@assign	 : ----------------
 */
void node_solve_path(t_array * nodes, INDEX s, INDEX t, FILE * stream);

/**
 *	@require : 	'n'		: un nombre de sommets
 *		   	'nodeSize'	: taille d'un sommet en mémoire
 *	@ensure	 : génère et renvoie un tableau de 'n' sommet non initialisé
 *	@assign	 : ----------------
 */
t_array * nodes_new(INDEX n, size_t nodeSize);
	
#endif
