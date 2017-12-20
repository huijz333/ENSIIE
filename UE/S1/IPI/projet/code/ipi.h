# ifndef IPI_H
# define IPI_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

typedef unsigned char	BYTE;
typedef BYTE 		BIT;

# define BITS_PER_BYTE (8 * sizeof(BYTE))

/* nombre maximal de sommets */
# define MAX_NODES (50)

/** type utilise pour representer l'indice des sommets
 *	(ici MAX_NODES == 50 => un BYTE suffit)
 *	avec son identifier pour 'printf()'
 */
typedef BYTE		INDEX;
# define INDEX_IDENTIFIER "%hhu"

# endif
