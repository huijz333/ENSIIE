/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#ifndef VEC3I_H
# define VEC3I_H

# include "cmaths.h"

typedef struct	s_vec3i {
	union {
		int x;
		int r;
		int pitch;
	};

	union {
		int y;
		int g;
		int yaw;
	};

	union {
		int z;
		int b;
		int roll;
	};
}				t_vec3i;

/** create a new vec3i */
t_vec3i * vec3i_new(void);

/** delete the vec3i */
void vec3i_delete(t_vec3i * vec);

/** set the vec3i to 0 */
t_vec3i * vec3i_zero(t_vec3i * dst);

/** set the vec3i values */
t_vec3i * vec3i_set(t_vec3i * dst, int x, int y, int z);
t_vec3i * vec3i_set3(t_vec3i * dst, t_vec3i * vec);

/** add two vec3i */
t_vec3i * vec3i_add(t_vec3i * dst, t_vec3i * left, t_vec3i * right);

/** sub two vec3i */
t_vec3i * vec3i_sub(t_vec3i * dst, t_vec3i * left, t_vec3i * right);

/** mult the vec3i by the given scalar */
t_vec3i * vec3i_mult(t_vec3i * dst, t_vec3i * vec, int scalar);
t_vec3i * vec3i_mult3(t_vec3i * dst, t_vec3i * left, t_vec3i * right);

/** cross product */
t_vec3i * vec3i_cross(t_vec3i * dst, t_vec3i * left, t_vec3i * right);

/** scale product */
int vec3i_dot_product(t_vec3i * left, t_vec3i * right);

/** length */
int vec3i_length_squared(t_vec3i * vec);
int vec3i_length(t_vec3i * vec);

/** normalize */
t_vec3i * vec3i_normalize(t_vec3i * dst, t_vec3i * vec);

/** negate */
t_vec3i * vec3i_negate(t_vec3i * dst, t_vec3i * src);

/** angle between two vec */
int vec3i_angle(t_vec3i * left, t_vec3i * right);

/** mix the two vectors */
t_vec3i * vec3i_mix(t_vec3i * dst, t_vec3i * left, t_vec3i * right, int ratio);

/** comparison */
int vec3i_equals(t_vec3i * left, t_vec3i * right);
int vec3i_nequals(t_vec3i * left, t_vec3i * right);

/** hash */
int vec3i_hash(t_vec3i * vec);

/** to string: return a string allocated with malloc() */
char * vec3i_str(t_vec3i * vec);

#endif