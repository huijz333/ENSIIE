/** definition des constantes du labyrinthe */

#ifndef LAB_DEF_H
# define LAB_DEF_H

# define LAB_EMPTY	('.')
# define LAB_ENTRY	('E')
# define LAB_EXIT	('S')
# define LAB_WALL	('X')

# define MAX_TP (11)
char * LAB_TP = "*%$#&+-@^!£";
# define MAX_DOORS (26)
char LAB_KEY_DOOR[MAX_DOORS][2] = {
	{'a', 'A'},
	{'b', 'B'},
	{'c', 'C'},
	{'d', 'D'},
	{'e', 'E'},
	{'f', 'F'},
	{'g', 'G'},
	{'h', 'H'},
	{'i', 'I'},
	{'j', 'J'},
	{'k', 'K'},
	{'l', 'L'},
	{'m', 'M'},
	{'n', 'N'},
	{'o', 'O'},
	{'p', 'P'},
	{'q', 'Q'},
	{'r', 'R'},
	{'s', 'S'},
	{'t', 'T'},
	{'u', 'U'},
	{'v', 'V'},
	{'w', 'W'},
	{'x', 'X'},
	{'y', 'Y'},
	{'z', 'Z'}
};

/** les directions possible de deplacement. */
typedef struct	s_direction {
	char const	* name;
	int		dx;
	int		dy;
}		t_direction;

static t_direction DIRECTIONS[4] = {
	{"DROITE",	 1,  0},
	{"GAUCHE",	-1,  0},
	{"HAUT",	 0,  1},
	{"BAS",		 0, -1}
};



#endif
