/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#ifndef VEC2F_H
# define VEC2F_H

# include "cmaths.h"

typedef struct	s_vec2f {
	union {
		float x;
		float uvx;
	};

	union {
		float y;
		float uvy;
	};
}				t_vec2f;

/** create a new vec2f */
t_vec2f * vec2f_new(void);

/** delete the vec2f */
void vec2f_delete(t_vec2f * vec);

/** set the vec2f to 0 */
t_vec2f * vec2f_zero(t_vec2f * dst);

/** set the vec2f values */
t_vec2f * vec2f_set(t_vec2f * dst, float x, float y);
t_vec2f * vec2f_set2(t_vec2f * dst, t_vec2f * vec);

/** add two vec2f */
t_vec2f * vec2f_add(t_vec2f * dst, t_vec2f * left, t_vec2f * right);

/** sub two vec2f */
t_vec2f * vec2f_sub(t_vec2f * dst, t_vec2f * left, t_vec2f * right);

/** mult the vec2f by the given scalar */
t_vec2f * vec2f_mult(t_vec2f * dst, t_vec2f * vec, float scalar);
t_vec2f * vec2f_mult2(t_vec2f * dst, t_vec2f * left, t_vec2f * right);

/** scale product */
float vec2f_dot_product(t_vec2f * left, t_vec2f * right);

/** length */
float vec2f_length_squared(t_vec2f * vec);
float vec2f_length(t_vec2f * vec);

/** normalize */
t_vec2f * vec2f_normalize(t_vec2f * dst, t_vec2f * vec);

/** negate */
t_vec2f * vec2f_negate(t_vec2f * dst, t_vec2f * src);

/** angle between two vec */
float vec2f_angle(t_vec2f * left, t_vec2f * right);

/** mix the two vectors */
t_vec2f * vec2f_mix(t_vec2f * dst, t_vec2f * left, t_vec2f * right, float ratio);

/** comparison */
int vec2f_equals(t_vec2f * left, t_vec2f * right);
int vec2f_nequals(t_vec2f * left, t_vec2f * right);

/** hash */
int vec2f_hash(t_vec2f * vec);

/** round vec2f */
t_vec2f * vec2f_round(t_vec2f * dst, t_vec2f * vec, int decimals);

/** to string: return a string allocated with malloc() */
char * vec2f_str(t_vec2f * vec);

#endif