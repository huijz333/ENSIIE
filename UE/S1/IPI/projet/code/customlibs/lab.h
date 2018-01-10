#ifndef LAB_H
# define LAB_H

# define _BSD_SOURCE
# include <unistd.h>	/* fork  */
# include <sys/types.h>	/* wait, kill */
# include <sys/wait.h>	/* wait */
# include <signal.h>	/* kill */


# include <stdio.h>	/* printf, scanf */
# include <stdlib.h>	/* free, malloc */
# include <string.h>	/* memcpy, memset... */
# include <limits.h>	/* ULONG_MAX ... */
# include <wchar.h>

# include "ipi.h"
# include "pqueue.h"
# include "list.h"

/** definitions des cases du labyrinthe */
# define LAB_EMPTY	('.')
# define LAB_ENTRY	('E')
# define LAB_EXIT	('S')
# define LAB_WALL	('X')
# define LAB_KEY	('a')
# define LAB_DOOR	('A')

/** chaine de caracteres contenant les teleporteurs */
# define MAX_TP (11)
extern wchar_t LAB_TP[MAX_TP];

/** les directions possible de deplacement. */
typedef struct	s_direction {
	char const	* name;
	int		x;
	int		y;
}		t_direction;
# define MAX_DIRECTIONS (4)
extern t_direction DIRECTIONS[MAX_DIRECTIONS];

/** represente une position dans la grille */
typedef struct	s_pos {
	INDEX	x, y;
}		t_pos;

/** represente un sommet du labyrinthe */
typedef struct	s_node {
	t_pos	pos;		/* la position sur la carte */
	INDEX	index;		/* index sur la carte */
	WEIGHT	f_cost;		/* poids == temps du chemin */
	WEIGHT	cost;		/* poids + heuristique du chemin */
	INDEX	prev;		/* predecesseur pour la remontée */
}		t_node;


/** represente le labyrinthe */
typedef struct	s_lab {
	wchar_t	** map;		/* wide char car '£' est un teleporteur */
	INDEX	width, height;	/* largeur et hauteur du labyrinthe */
	t_pos	entry, exit;	/* position entrée et sortie */
	t_pos	key, door;	/* position de la clef et de la porte */
	t_pos	tps[MAX_TP][2];	/* position teleporteurs */
}		t_lab;

/**
 *	@require :	l : longueur (== largeur) du labyrinthe
 *	@ensure	 :	crée un nouveau labyrinthe de taille 'l'
 *	@assign  :	-------
 */
t_lab * lab_new(int l);

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

/**
 *	@require : un caractère 'c' de la chaine 'LAB_TP'
 *	@ensure  : renvoie l'id 'i' dans le tableau t_lab.tps[i] correspondant
 *		   au teleporteur du caractère 'c', ou 'MAX_TP' si le teleporteur
 *		   n'existe pas.
 *	@assign  : --------------
 */
BYTE lab_get_tpID(wchar_t c);

/**
 *	@require : le labyrinthe, et le timeout
 *	@ensure  : tente de résoudre le labyrinthe dans le temps imparti.
 *		   	1) s'il n'y a pas de portes, on résout avec A*
 *			2) sinon, on résout en parallèle les 4 problemes de
 *			chemin suivant, avec A*:
 *				1.1) chemin ne passant pas par la porte
 *				1.2) chemin allant du départ à la clé
 *				1.3) chemin allant de la clé à la porte
 *				1.4) chemin allant de la porte à la clef
 *			La parallélisation s'arrête si 1) convient,
 *			ou si la concaténation de 2), 3) et 4) convient.
 *
 *			Si un chemin convient, le signal 'SIG_SUCCESS' est
 *			envoyé à l'enfant, qui affiche son chemin.
 *
 *			et alors, le signal 'SIGKILL' est envoyé aux autres.
 *
 *		Renvoie 1 si un chemin a été trouvé et affiché,
 *		sinon, 0 est renvoyé et "Not connected" est affiché
 *	@assign  : -----------------------------------------
 */
int lab_solve(t_lab * lab, WEIGHT timer);

/**
 *	@require : le labyrinthe, les sommets, un sommet de
 *		   départ 's', et un sommet d'arrivée 't'.
 *	@ensure  : effectue l'algorithme de remontée de 's' à 't',
 *		   et affiche les directions successives à prendre
 *		   pour realiser le chemin.
 *	@assign  : ------------------
 */
void lab_print_path(t_lab * lab, t_node * nodes, INDEX s, INDEX t);


/** inclut à la fin, car requiet les définitions précèdentes */
# include "astar.h"
# include "astar_client.h"

#endif
