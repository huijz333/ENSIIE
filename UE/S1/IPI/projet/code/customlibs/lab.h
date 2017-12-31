#ifndef LAB_H
# define LAB_H

# include "ipi.h"
# include "astar.h"
# include "dijkstra.h"

/** definitions des cases du labyrinthe */
# define LAB_EMPTY	('.')
# define LAB_ENTRY	('E')
# define LAB_EXIT	('S')
# define LAB_WALL	('X')
# define LAB_KEY	('a')
# define LAB_DOOR	('A')

# define MAX_TP (12)
extern char LAB_TP[MAX_TP];

/** les directions possible de deplacement. */
typedef struct	s_direction {
	char const	* name;
	int		x;
	int		y;
}		t_direction;

# define MAX_DIRECTIONS (4)
extern t_direction DIRECTIONS[MAX_DIRECTIONS];

/** represente un sommet dans le graphe representant le labyrinthe */
typedef struct	s_nodel {
	t_nodew	super;	/** héritage 'à la C' */
	INDEX	x, y;	/** position x, y dans le labyrinthe */
	BYTE	id;	/** id du sommet (character dans la map) */
}		t_nodel;

/** represente le labyrinthe */
typedef struct	s_lab {
	t_array	* nodes;	/* le graphe */
	INDEX	l;		/* largeur == longueur du labyrinthe */
	INDEX	entry, exit;	/* indice d'entrée 's', et de sortie 't' du labyrinthe */
	INDEX	key, door;	/* indice dans le graphe de la clef et de la porte */
	INDEX	vdoor[MAX_DIRECTIONS]; /* index des voisins de la porte */
}		t_lab;

/**
 *	@require :	n : longueur (== largeur) du labyrinthe
 *	@ensure	 :	crée un nouveau labyrinthe de taille 'n'
 *	@assign  :	-------
 */
t_lab * lab_new(INDEX n);

/**
 *	@require :	lab : un labyrinthe alloué via 'lab_new()'
 *	@ensure	 :	supprime le labyrinthe du tas
 *	@assign  :	-------
 */
void lab_delete(t_lab * lab);

/**
 *	@require :	lab : un labyrinthe alloué via 'lab_new()'
 *	@ensure	 :	lit un labyrinthe sur l'entrée standart
 *	@assign  :	-------
 */
t_lab * lab_parse(void);

int lab_solve(t_lab * lab, WEIGHT timer);

#endif
