#ifndef LAB_NODE_H
# define LAB_NODE_H

# include "node.h"

/** sommet du labyrinthe */
# define NODE_ID_EMPTY	(0)
# define NODE_ID_TP	(1)
# define NODE_ID_KEY	(2)
# define NODE_ID_DOOR	(3)

/** represente un sommet dans le graphe representant le labyrinthe */
typedef struct	s_nodel {
	t_nodew	super;	/* héritage 'à la C' */
	INDEX	x, y;	/* position x, y dans le labyrinthe */
	BYTE	id;	/* ID indiquant le type de sommet (vide, tp, porte, clef) */
}		t_nodel;

/** teleporteur */
typedef struct	s_nodetp {
	t_nodel	super;
	INDEX	tx, ty;	/* position du teleporteurs lié à celui ci */
}		t_nodetp;

/** clef */
typedef t_nodel t_nodek;

/** porte */
typedef struct	s_noded {
	t_nodel	super;
	INDEX	keyx, keyy;/* clef requit pour ouvrir cette porte */
}		t_noded;

/** represente le labyrinthe */
typedef struct	s_lab {
	INDEX	l;	/* largeur == longueur du labyrinthe */
	t_array	* nodes;/* le graphe */
}		t_lab;



#endif
