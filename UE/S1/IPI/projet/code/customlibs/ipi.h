#ifndef IPI_H
# define IPI_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>	/* UCHAR_MAX ... */

/* typedef pratique ... */
# ifndef BYTE
#	define BYTE unsigned char
# endif
# ifndef BIT
#	define BIT	BYTE
# endif

# define BITS_PER_BYTE (8 * sizeof(BYTE))

#endif	/* ifndef IPI_H */
