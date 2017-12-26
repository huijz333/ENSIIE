#ifndef NODE_H
# define NODE_H

# include "ipi.h"
# include "array.h"

/**
 *	Represente un sommet du graphe (exo 1)
 */
typedef struct	s_node {
	t_array	* successors; /* les successeurs de ce sommet */
	INDEX	pathlen;/* nombre de sommets entre s et ce sommet */
	INDEX	prev;	/* element precedant le chemin (index du sommet) */

}		t_node;

/**
 *	Represente un sommet du graphe, avec un chemin pondéré (exo 2)
 */
typedef struct	s_nodew {
	t_node	super;	/* héritage 'à la C' */	
	t_array	* ws;	/* poids des arcs (<=> index des successeurs) */
}		t_nodew;

/**
 *	Represente un sommet du graphe du labyrinthe (exo 3)
 */
typedef struct	s_nodel {
	t_nodew	super;	/* héritage 'à la C' */
	INDEX	x, y;	/* position x, y dans le labyrinthe */
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

#endif
