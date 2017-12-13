#ifndef EXO_2
# define EXO_2

# include "ipi.h"

typedef struct	s_node {
	BIT		visited;
	unsigned int	d;
	INDEX		path[MAX_NODES];
	INDEX		path_len;
}		t_node;


#endif
