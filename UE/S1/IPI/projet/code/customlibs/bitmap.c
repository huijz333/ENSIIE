# include "bitmap.h"

/**
 *	@require: le nombre de bit 'n'
 *	@ensure : alloue en mémoire un tableau d'au moins 'size' BIT
 *			(arrondi au BITMAP_UNIT supérieur)
 *	@assign : -------------
 */
t_bitmap * bitmap_new(INDEX n) {
	/* 'bitsize' : nombre de bit requis */
	size_t bitsize = n * n;
	/* 'arcsize' : nombre de BITMAP_UNIT minimum requit pour stocker les bits */
	size_t unitsize = (bitsize / BITS_PER_UNIT) + ((bitsize % BITS_PER_UNIT) != 0);
	
	t_bitmap * bitmap = (t_bitmap *) malloc(sizeof(t_bitmap) + unitsize);
	if (bitmap == NULL) {
		return (NULL);
	}
	bitmap->n = n;
	memset(bitmap + 1, 0, unitsize);
	return (bitmap);
}

/**
 *	@require: une bitmap alloué avec 'bitmap_new()'
 *	@ensure : supprime de la mémoire la bitmap
 *	@assign : -------------
 */
void bitmap_delete(t_bitmap * bitmap) {
	free(bitmap);
}

/**
 *	@require: une bitmap alloué avec 'bitmap_new()'
 *	@ensure : supprime de la mémoire la bitmap
 *	@assign : -------------
 */
BIT bitmap_get(t_bitmap * bitmap, INDEX x, INDEX y) {
	/**
	 * NB1:	on a ici une division, ce qui ralentirait théoriquement l'acces
	 *	en lecture et en ecriture aux donnees.
	 *	Cependant, BITS_PER_UNIT est une puissance de 2, donc
	 *	le compilateur saura optimiser cette division par un decalage de bit.
	 *	(BITS_PER_UNIT vaux 8, diviser par 8 est équivalent a un decalage
	 *	de 3 bits vers la droite...)
	 *
	 * NB2:	De plus, utilisé moins de mémoire permet de rendre le programme
	 *	'cache-friendly', à savoir, que la mémoire du tas est copié
	 *	dans la mémoire cache du processeur, la rendant très rapide d'accès.
	 *	En réduisant la mémoire, on s'évite ainsi de nombreux aller/retour
	 *	dans la mémoire.
	 */
	size_t bit = y * bitmap->n + x;
	BYTE * bits = (BYTE *) (bitmap + 1);
	return (bits[bit / BITS_PER_UNIT] & (1 << (bit % BITS_PER_UNIT)) ? 1 : 0);

}

/**
 *	@require: une bitmap, et les indices du bit
 *	@ensure : met le bit à 1
 *	@assign : -------------
 */
void bitmap_set(t_bitmap * bitmap, INDEX x, INDEX y) {
	size_t bit = y * bitmap->n + x;
	BYTE * bits = (BYTE *) (bitmap + 1);
	*(bits + bit / BITS_PER_UNIT) |= (1 << (bit % BITS_PER_UNIT));
}

/**
 *	@require: une bitmap, et les indices du bit
 *	@ensure : met le bit à 0
 *	@assign : -------------
 */
void bitmap_unset(t_bitmap * bitmap, INDEX x, INDEX y) {
	size_t bit = y * bitmap->n + x;
	BYTE * bits = (BYTE *) (bitmap + 1);
	*(bits + bit / BITS_PER_UNIT) &= ~(1 << (bit % BITS_PER_UNIT));
}

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
t_bitmap * bitmap_parse(FILE * stream) {
	INDEX n;
	fscanf(stream, INDEX_IDENTIFIER, &n);
	t_bitmap * bitmap = bitmap_new(n);
	if (bitmap == NULL) {
		return (NULL);
	}

	INDEX i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			BYTE is_one;
			fscanf(stream, "%hhu", &is_one);
			if (is_one) {
				bitmap_set(bitmap, i, j);
			}
		}
	}
	return (bitmap);
}

/**
 *	@require: une bitmap, et un flux
 *	@ensure : ecrit la bitmap sur le flux, et renvoie le nombre d'octet ecrit
 *	@assign : -------------
 */
int bitmap_write(t_bitmap * bitmap, FILE * stream) {
	int r = 0;
	INDEX i, j;
	for (i = 0 ; i < bitmap->n ; i++) {
		for (j = 0 ; j < bitmap->n - 1; j++) {
			r += fprintf(stream, "%c ", bitmap_get(bitmap, i, j) ? '1' : '0');
		}
		r += fprintf(stream, "%c\n", bitmap_get(bitmap, i, j) ? '1' : '0');
	}
	return (r);
}
