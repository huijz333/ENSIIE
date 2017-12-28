#ifndef IPI_H
# define IPI_H

# include <stdio.h>	/* printf, scanf */
# include <stdlib.h>	/* free, malloc */
# include <string.h>	/* memcpy, memset... */
# include <limits.h>	/* UCHAR_MAX ... */
# include <float.h>	/* DBL_MAX ... */

/* typedef pratique ... */
# ifndef BYTE
#	define BYTE	unsigned char
# endif
# ifndef BIT
#	define BIT	BYTE
# endif

/**
 *	MAX_NODES	: nombre maximal de sommets.
 *	INDEX		: type a utilisé pour stocker un indice d'un sommet
 *	INDEX_IDENTIFIER: printf(), scanf() identifier pour le type INDEX
 */
# ifndef MAX_NODES
	/**
	 *	si aucunes tailles n'est precisé, utilisé la taille maximal
	 *	des exos 1, 2 et 3, à savoir, dans l'exo 3, 1000 * 1000
	 *	si on considère chaque case comme un sommet
	 *
	 *	NB: gcc [FLAGS] -D MAX_NODES=50
	 */
#	define MAX_NODES (1000 * 1000)
/*#	pragma message("\nValue par défaut utilisée pour MAX_NODES (= 1000 * 1000)\n"\
			"Vous pouvez definir une valeur plus basse pour optimiser "\
			"l'usage mémoire\nou plus grande si l'entrée contient "\
			"plus de 1000 * 1000 sommets.\ne.g: gcc [FLAGS] -D MAX_NODES=50")
*/
# endif

# if MAX_NODES < UCHAR_MAX
#	define INDEX		unsigned char
#	define INDEX_IDENTIFIER "%hhu"
# elif MAX_NODES < USHRT_MAX
#	define INDEX		unsigned short
#	define INDEX_IDENTIFIER	"%hu"
# elif MAX_NODES < UINT_MAX
#	define INDEX		unsigned int
#	define INDEX_IDENTIFIER	"%u"
# else
#	define INDEX		size_t
#	define INDEX_IDENTIFIER	"%lu"
# endif

/**
 *	Le type a utilisé pour modélisé les poids sur les arcs
 */
# define WEIGHT double
# define INF_WEIGHT DBL_MAX

#endif	/* ifndef IPI_H */
