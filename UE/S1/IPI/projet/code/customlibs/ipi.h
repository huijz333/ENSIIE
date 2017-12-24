#ifndef IPI_H
# define IPI_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>	/* UCHAR_MAX ... */
# include <unistd.h>

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
# if defined(EXO1_H) || defined(EXO2_H) /* pour l'exo 1 et 2, on a 50 sommets max ... */
#	define MAX_NODES 		(50)
#	define INDEX			BYTE
#	define INDEX_IDENTIFIER 	"%hhu"
# else /* pour l'exo 3, on a 1000 sommets max */

/**	NB:	on pourrait décider de les codder sur 10 bits (1000 < 1024),
 *		en utilisant des tableaux de bits sur le modèle de l'exo 1.
 *		(ce qui permettrait de gagner de la mémoire)
 *		mais cela complexifierait énormement le code, et ferait perdre
 *		beaucoup de temps de conversion. On le code donc sur 16 bits
 */
#	define MAX_NODES 		(1000)
#	define INDEX			unsigned short
#	define INDEX_IDENTIFIER 	"%hu"
# endif

#endif	/* ifndef IPI_H */
