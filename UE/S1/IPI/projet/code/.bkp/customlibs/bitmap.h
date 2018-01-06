#ifndef BITMAP_H
# define BITMAP_H

# include "ipi.h"

/**
 *	Le type primitif utilisé pour le tableau de bit
 */
# ifndef BITMAP_UNIT
# 	define BITMAP_UNIT BYTE
# endif
# define BITS_PER_UNIT (sizeof(BITMAP_UNIT) * 8)

/**
 *	Un tableau de 'size' bits
 */
typedef struct	s_bitmap {
	INDEX	capacity;
}		t_bitmap;

/**
 *	@require: le nombre de bit 'capacity'
 *	@ensure : alloue en mémoire un tableau d'au moins 'capacity' bit
 *			(arrondi au BITMAP_UNIT supérieur)
 *	@assign : -------------
 */
t_bitmap * bitmap_new(INDEX capacity);

/**
 *	@require: une bitmap alloué avec 'bitmap_new()'
 *	@ensure : supprime de la mémoire la bitmap
 *	@assign : -------------
 */
void bitmap_delete(t_bitmap * bitmap);

/**
 *	@require: une bitmap alloué avec 'bitmap_new()', et l'indice du bit
 *	@ensure : renvoie 1 si le bit est défini, 0 sinon
 *	@assign : -------------
 */
BIT bitmap_get(t_bitmap * bitmap, INDEX i);

/**
 *	@require: une bitmap, et l'indice du bit
 *	@ensure : met le bit à 1
 *	@assign : -------------
 */
void bitmap_set(t_bitmap * bitmap, INDEX i);

/**
 *	@require: une bitmap, et l'indice du bit
 *	@ensure : met le bit à 0
 *	@assign : -------------
 */
void bitmap_unset(t_bitmap * bitmap, INDEX i);


t_bitmap * bitmap_parse2(FILE * stream);

/**
 *	@require: une bitmap, et un flux
 *	@ensure : ecrit la bitmap sur le flux, et renvoie le nombre d'octet ecrit
 *	@assign : -------------
 */
int bitmap_write(t_bitmap * bitmap, FILE * stream);

/**
 *	fonctions similaires, mais en appliquant la bijection:
 *
 *		i : (x, y) -> y * width + x
 *
 *		 0    1   ...   k   ...   n
 *		n+1  n+2  ...  k+n  ...  2n-1
 *		          ........
 *		          ........
 *	        	  ........
 *		n(n-1)    ........	 n.n
 */

t_bitmap * bitmap_new2(INDEX width, INDEX height);
BIT bitmap_get2(t_bitmap * bitmap, INDEX x, INDEX y, INDEX width);
void bitmap_set2(t_bitmap * bitmap, INDEX x, INDEX y, INDEX width);
void bitmap_unset2(t_bitmap * bitmap, INDEX x, INDEX y, INDEX width);

# endif
