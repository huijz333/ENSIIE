# include "bitmap.h"

/**
 *	@require: le nombre de bit 'n'
 *	@ensure : alloue en mémoire un tableau d'au moins 'size' BIT
 *			(arrondi au BITMAP_UNIT supérieur)
 *	@assign : -------------
 */
t_bitmap * bitmap_new(INDEX capacity) {
	/* 'bitsize' : nombre de bit requis */
	size_t bitsize = capacity;
	/* 'unitcount' : nombre de BITMAP_UNIT minimum requit pour stocker les bits */
	size_t unitcount = (bitsize / BITS_PER_UNIT) + ((bitsize % BITS_PER_UNIT) != 0);
	size_t unitsize = unitcount * sizeof(BITMAP_UNIT);
	
	t_bitmap * bitmap = (t_bitmap *) malloc(sizeof(t_bitmap) + unitsize);
	if (bitmap == NULL) {
		return (NULL);
	}
	bitmap->capacity = capacity;
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
BIT bitmap_get(t_bitmap * bitmap, INDEX bit) {
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
	BITMAP_UNIT * bits = (BITMAP_UNIT *) (bitmap + 1);
	return (bits[bit / BITS_PER_UNIT] & (1 << (bit % BITS_PER_UNIT)) ? 1 : 0);

}

/**
 *	@require: une bitmap, et les indices du bit
 *	@ensure : met le bit à 1
 *	@assign : -------------
 */
void bitmap_set(t_bitmap * bitmap, INDEX bit) {
	BITMAP_UNIT * bits = (BITMAP_UNIT *) (bitmap + 1);
	*(bits + bit / BITS_PER_UNIT) |= (1 << (bit % BITS_PER_UNIT));
}

/**
 *	@require: une bitmap, et les indices du bit
 *	@ensure : met le bit à 0
 *	@assign : -------------
 */
void bitmap_unset(t_bitmap * bitmap, INDEX bit) {
	BITMAP_UNIT * bits = (BITMAP_UNIT *) (bitmap + 1);
	*(bits + bit / BITS_PER_UNIT) &= ~(1 << (bit % BITS_PER_UNIT));
}

/**
 *	fonctions similaires, mais en appliquant la bijection:
 *
 *		i : (x, y) -> y * width + x
 */
static INDEX bitmap_index(INDEX x, INDEX y, INDEX width) {
	return (y * width + x);
}

t_bitmap * bitmap_new2(INDEX width, INDEX height) {
	return (bitmap_new(width * height));
}

BIT bitmap_get2(t_bitmap * bitmap, INDEX x, INDEX y, INDEX width) {
	return (bitmap_get(bitmap, bitmap_index(x, y, width)));
}

void bitmap_set2(t_bitmap * bitmap, INDEX x, INDEX y, INDEX width) {
	bitmap_set(bitmap, bitmap_index(x, y, width));
}

void bitmap_unset2(t_bitmap * bitmap, INDEX x, INDEX y, INDEX width) {
	bitmap_unset(bitmap, bitmap_index(x, y, width));
}
