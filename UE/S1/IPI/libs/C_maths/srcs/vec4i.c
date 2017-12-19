/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#include "vec4i.h"

/** create a new vec4i */
t_vec4i * vec4i_new(void) {
	return ((t_vec4i *)malloc(sizeof(t_vec4i)));
}

/** delete the vec4i */
void vec4i_delete(t_vec4i * vec) {
	free(vec);
}

/** set the vec4i to 0 */
t_vec4i * vec4i_zero(t_vec4i * dst) {
	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}
	memset(dst, 0, sizeof(t_vec4i));
	return (dst);
}

/** set the vec4i values */
t_vec4i * vec4i_set(t_vec4i * dst, int x, int y, int z, int w) {
	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = x;
	dst->y = y;
	dst->z = z;
	dst->w = w;
	return (dst);
}

t_vec4i * vec4i_set4(t_vec4i * dst, t_vec4i * vec) {
	if (dst == vec) {
		return (dst);
	}
	return (vec4i_set(dst, vec->x, vec->y, vec->z, vec->w));
}

/** add two vec4i */
t_vec4i * vec4i_add(t_vec4i * dst, t_vec4i * left, t_vec4i * right) {
	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x + right->x;
	dst->y = left->y + right->y;
	dst->z = left->z + right->z;
	dst->w = left->w + right->w;
	return (dst);
}

/** sub two vec4i */
t_vec4i * vec4i_sub(t_vec4i * dst, t_vec4i * left, t_vec4i * right) {
	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x - right->x;
	dst->y = left->y - right->y;
	dst->z = left->z - right->z;
	dst->w = left->w - right->w;
	return (dst);
}

/** mult the vec4i by the given scalar */
t_vec4i * vec4i_mult(t_vec4i * dst, t_vec4i * vec, int scalar) {
	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = vec->x * scalar;
	dst->y = vec->y * scalar;
	dst->z = vec->z * scalar;
	dst->w = vec->w * scalar;
	return (dst);
}

t_vec4i * vec4i_mult3(t_vec4i * dst, t_vec4i * left, t_vec4i * right) {
	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x * right->x;
	dst->y = left->y * right->y;
	dst->z = left->z * right->z;
	dst->w = left->w * right->w;
	return (dst);
}

/** scale product */
int vec4i_dot_product(t_vec4i * left, t_vec4i * right) {
	return (left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w);
}

/** length */
int vec4i_length_squared(t_vec4i * vec) {
	return (vec4i_dot_product(vec, vec));
}

int vec4i_length(t_vec4i * vec) {
	return ((int)sqrt(vec4i_length_squared(vec)));
}

/** normalize */
t_vec4i * vec4i_normalize(t_vec4i * dst, t_vec4i * vec) {

	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}

	int norm = 1 / vec4i_length(vec);
	dst->x = vec->x * norm;
	dst->y = vec->y * norm;
	dst->z = vec->z * norm;
	dst->w = vec->w * norm;
	return (dst);
}


/** negate */
t_vec4i * vec4i_negate(t_vec4i * dst, t_vec4i * src) {
	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = -src->x;
	dst->y = -src->y;
	dst->z = -src->z;
	dst->w = -src->w;
	return (dst);
}

/** angle between two vec */
int vec4i_angle(t_vec4i * left, t_vec4i * right) {
	int dls = vec4i_dot_product(left, right) / (vec4i_length(left) * vec4i_length(right));
	if (dls < -1.0f) {
		dls = -1.0f;
	} else if (dls > 1.0f) {
		dls = 1.0f;
	}
	return ((int)acos(dls));
}

/** mix the two vectors */
t_vec4i * vec4i_mix(t_vec4i * dst, t_vec4i * left, t_vec4i * right, int ratio) {

	if (dst == NULL) {
		if ((dst = vec4i_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = left->x * ratio + right->x * (1 - ratio);
	dst->y = left->y * ratio + right->y * (1 - ratio);
	dst->z = left->z * ratio + right->z * (1 - ratio);
	dst->w = left->w * ratio + right->w * (1 - ratio);
	return (dst);
}

/** comparison */
int vec4i_equals(t_vec4i * left, t_vec4i * right) {
	return (left == right || (left->x == right->x && left->y == right->y && left->z == right->z && left->w == right->w));
}

int vec4i_nequals(t_vec4i * left, t_vec4i * right) {
	return (!vec4i_equals(left, right));
}

/** hash */
int vec4i_hash(t_vec4i * vec) {
	return ((vec->x * 73856093) ^ (vec->y * 19349663) ^ (vec->z * 83492791) ^ (vec->w * 3539857));
}

/** to string: return a string allocated with malloc() */
char * vec4i_str(t_vec4i * vec) {
	if (vec == NULL) {
		return (strdup("vec4i(NULL)"));
	}
	char buffer[256];
	sprintf(buffer, "vec4i(%d ; %d ; %d ; %d)", vec->x, vec->y, vec->z, vec->w);
	return (strdup(buffer));
}
