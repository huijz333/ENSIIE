/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#ifndef VEC4F_H
# define VEC4F_H

# include "cmaths.h"

typedef struct	s_vec4f {
	union {
		float x;
		float r;
	};

	union {
		float y;
		float g;
	};

	union {
		float z;
		float b;
	};

	union {
		float w;
		float a;
	};
}				t_vec4f;

/** create a new vec4f */
t_vec4f * vec4f_new(void);

/** delete the vec4f */
void vec4f_delete(t_vec4f * vec);

/** set the vec4f to 0 */
t_vec4f * vec4f_zero(t_vec4f * dst);

/** set the vec4f values */
t_vec4f * vec4f_set(t_vec4f * dst, float x, float y, float z, float w);
t_vec4f * vec4f_set4(t_vec4f * dst, t_vec4f * vec);

/** add two vec4f */
t_vec4f * vec4f_add(t_vec4f * dst, t_vec4f * left, t_vec4f * right);

/** sub two vec4f */
t_vec4f * vec4f_sub(t_vec4f * dst, t_vec4f * left, t_vec4f * right);

/** mult the vec4f by the given scalar */
t_vec4f * vec4f_mult(t_vec4f * dst, t_vec4f * vec, float scalar);
t_vec4f * vec4f_mult2(t_vec4f * dst, t_vec4f * left, t_vec4f * right);

/** scale product */
float vec4f_dot_product(t_vec4f * left, t_vec4f * right);

/** length */
float vec4f_length_squared(t_vec4f * vec);
float vec4f_length(t_vec4f * vec);

/** normalize */
t_vec4f * vec4f_normalize(t_vec4f * dst, t_vec4f * vec);

/** negate */
t_vec4f * vec4f_negate(t_vec4f * dst, t_vec4f * src);

/** angle between two vec */
float vec4f_angle(t_vec4f * left, t_vec4f * right);

/** mix the two vectors */
t_vec4f * vec4f_mix(t_vec4f * dst, t_vec4f * left, t_vec4f * right, float ratio);

/** comparison */
int vec4f_equals(t_vec4f * left, t_vec4f * right);
int vec4f_nequals(t_vec4f * left, t_vec4f * right);

/** hash */
int vec4f_hash(t_vec4f * vec);

/** round vec4f */
t_vec4f * vec4f_round(t_vec4f * dst, t_vec4f * vec, int decimals);

/** to string: return a string allocated with malloc() */
char * vec4f_str(t_vec4f * vec);

#endif