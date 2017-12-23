#ifndef EXO3_H
# define EXO3_H

# include "hmap.h"
# include "ipi.h"

/**
 *	MAX_BOXES	: largeur (== hauteur) du labyrinthe
 *	INDEX		: type a utilisé pour stocker un indice d'une case
 *	INDEX_IDENTIFIER: printf(), scanf() identifier pour le type INDEX
 *
 *	NB:	on pourrait décider de les codder sur 10 bits (1000 < 1024),
 *		en utilisant des tableaux de bits sur le modèle de l'exo 1.
 *		(ce qui permettrait de gagner de la mémoire)
 *		mais cela complexifierait énormement le code, et ferait perdre
 *		beaucoup de temps de conversion
 */
# define MAX_BOXES 		(1000)
# define INDEX			unsigned short
# define INDEX_IDENTIFIER 	"%hu"

/** represente un téléporteur, avec ces 2 points */
typedef struct	s_teleporter {
	INDEX x1, y1;
	INDEX x2, y2;
}		t_teleporter;

/** represente une clef et une porte */
typedef struct	s_door {
	INDEX xkey, ykey;
	INDEX xdoor, ydoor;
}		t_door;

/** represente le labyrinthe */
typedef struct	s_lab {
	BYTE	* obstacles;
	t_hmap	* teleporters;
	t_hmap	* doors;
}		t_lab;

#endif
