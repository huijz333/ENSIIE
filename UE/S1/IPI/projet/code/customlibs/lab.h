#ifndef LAB_H
# define LAB_H

# include "ipi.h"
# include "hmap.h"
# include "astar.h"

# define LAB_CHAR_EMPTY	('.')
# define LAB_CHAR_WALL	('X')
# define LAB_CHARSET_KEYS  ("abcdefghijklmnopqrstuvwxyz")
# define LAB_CHARSET_DOORS ("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
# define LAB_CHARSET_TELEPORTERS ("*%$#&+-@^£!")

/** represente le labyrinthe */
typedef struct	s_lab {
	INDEX	l;	/* largeur == longueur du labyrinthe */
	t_array	* nodes;/* le graphe */
	t_hmap	* teleporters;
	t_hmap	* doors;
}		t_lab;

/**
 *	NB : ce constructeur est relativement long, car on s'assure
 *		qu'il n'y ait pas de fuite mémoire en cas d'erreur
 *
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
t_lab * lab_parse(FILE * stream);

void lab_solve(t_lab * lab, unsigned int timer);

#endif
