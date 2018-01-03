#ifndef LAB_H
# define LAB_H

# include <stdio.h>	/* printf, scanf */
# include <stdlib.h>	/* free, malloc */
# include <string.h>	/* memcpy, memset... */
# include <unistd.h>	/* fork */
# include <sys/types.h>	/* pid_t*/
# include <signal.h>	/* pid_t */
# include <sys/mman.h>	/*mmap */
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

typedef struct	s_pos {
	INDEX	x, y;
}		t_pos;

/** represente un sommet du labyrinthe */
typedef struct	s_node {
	t_pos		pos;		/* la position sur la carte */
	INDEX		index;		/* index sur la carte */
	WEIGHT		f_cost;		/* poids == temps du chemin */
	WEIGHT		g_cost;		/* heuristique du chemin */
	struct s_node	* prev;		/* predecesseur pour la remontée */
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

int lab_solve(t_lab * lab, size_t timer);
BYTE lab_get_tpID(wchar_t c);

#endif
