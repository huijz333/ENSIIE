#ifndef LAB_H
# define LAB_H

# include "ipi.h"
# include "hmap.h"
# include "astar.h"
# include "lab_def.h"
# include "lab_node.h"

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
t_lab * lab_parse(FILE * stream);

void lab_solve(t_lab * lab, unsigned int timer);

#endif
