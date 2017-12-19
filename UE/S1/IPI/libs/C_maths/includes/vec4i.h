/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#ifndef VEC4I_H
# define VEC4I_H
 
# include "cmaths.h"

typedef struct	s_vec4i {
	union {
		int x;
		int r;
	};

	union {
		int y;
		int g;
	};

	union {
		int z;
		int b;
	};

	union {
		int w;
		int a;
	};
}				t_vec4i;

/** create a new vec4i */
t_vec4i * vec4i_new(void);

/** delete the vec4i */
void vec4i_delete(t_vec4i * vec);

/** set the vec4i to 0 */
t_vec4i * vec4i_zero(t_vec4i * dst);

/** set the vec4i values */
t_vec4i * vec4i_set(t_vec4i * dst, int x, int y, int z, int w);
t_vec4i * vec4i_set4(t_vec4i * dst, t_vec4i * vec);

/** add two vec4i */
t_vec4i * vec4i_add(t_vec4i * dst, t_vec4i * left, t_vec4i * right);

/** sub two vec4i */
t_vec4i * vec4i_sub(t_vec4i * dst, t_vec4i * left, t_vec4i * right);

/** mult the vec4i by the given scalar */
t_vec4i * vec4i_mult(t_vec4i * dst, t_vec4i * vec, int scalar);
t_vec4i * vec4i_mult2(t_vec4i * dst, t_vec4i * left, t_vec4i * right);

/** scale product */
int vec4i_dot_product(t_vec4i * left, t_vec4i * right);

/** length */
int vec4i_length_squared(t_vec4i * vec);
int vec4i_length(t_vec4i * vec);

/** normalize */
t_vec4i * vec4i_normalize(t_vec4i * dst, t_vec4i * vec);

/** negate */
t_vec4i * vec4i_negate(t_vec4i * dst, t_vec4i * src);

/** angle between two vec */
int vec4i_angle(t_vec4i * left, t_vec4i * right);

/** mix the two vectors */
t_vec4i * vec4i_mix(t_vec4i * dst, t_vec4i * left, t_vec4i * right, int ratio);

/** comparison */
int vec4i_equals(t_vec4i * left, t_vec4i * right);
int vec4i_nequals(t_vec4i * left, t_vec4i * right);

/** hash */
int vec4i_hash(t_vec4i * vec);

/** to string: return a string allocated with malloc() */
char * vec4i_str(t_vec4i * vec);

#endif