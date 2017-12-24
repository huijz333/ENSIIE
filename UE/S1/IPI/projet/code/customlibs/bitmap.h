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
 *	Un tableau 2D de n * n bits
 *	chaque bit est indexé par un doublet (x, y)
 *	on associe le ième (i = y * n + x) bit au couple (x, y)
 */
typedef struct	s_bitmap {
	INDEX	n;
}		t_bitmap;

/**
 *	@require: le nombre de bit 'n'
 *	@ensure : alloue en mémoire un tableau d'au moins 'size' BIT
 *			(arrondi au BITMAP_UNIT supérieur)
 *	@assign : -------------
 */
t_bitmap * bitmap_new(INDEX n);

/**
 *	@require: une bitmap alloué avec 'bitmap_new()'
 *	@ensure : supprime de la mémoire la bitmap
 *	@assign : -------------
 */
void bitmap_delete(t_bitmap * bitmap);

/**
 *	@require: une bitmap alloué avec 'bitmap_new()'
 *	@ensure : supprime de la mémoire la bitmap
 *	@assign : -------------
 */
BIT bitmap_get(t_bitmap * bitmap, INDEX x, INDEX y);

/**
 *	@require: une bitmap, et les indices du bit
 *	@ensure : met le bit à 1
 *	@assign : -------------
 */
void bitmap_set(t_bitmap * bitmap, INDEX x, INDEX y);

/**
 *	@require: une bitmap, et les indices du bit
 *	@ensure : met le bit à 0
 *	@assign : -------------
 */
void bitmap_unset(t_bitmap * bitmap, INDEX x, INDEX y);

/**
 *	@require : des données bien formattés sur le flux donnée
 *	@ensure  : lit un tableau de bit sur le flux
 *			Le 1er entier lu est la dimension 'n',
 *			suivi de n * n valeurs, où la k-ieme valeur correspond
 *			a la ligne 'k / n' et a la colonne 'k % n'
 *
 *			 0    1   ...   k   ...   n
 *			n+1  n+2  ...  k+n  ...  2n-1
 *			          ........
 *			          ........
 *		        	  ........
 *			n(n-1)    ........	 n.n
 *
 *	@assign  : -----------------------
*/
t_bitmap * bitmap_parse(FILE * stream);

/**
 *	@require: une bitmap, et un flux
 *	@ensure : ecrit la bitmap sur le flux, et renvoie le nombre d'octet ecrit
 *	@assign : -------------
 */
int bitmap_write(t_bitmap * bitmap, FILE * stream);

# endif
