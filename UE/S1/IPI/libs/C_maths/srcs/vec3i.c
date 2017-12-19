/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#include "vec3i.h"

/** create a new vec3i */
t_vec3i * vec3i_new(void) {
	return ((t_vec3i *)malloc(sizeof(t_vec3i)));
}

/** delete the vec3i */
void vec3i_delete(t_vec3i * vec) {
	free(vec);
}

/** set the vec3i to 0 */
t_vec3i * vec3i_zero(t_vec3i * dst) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	memset(dst, 0, sizeof(t_vec3i));
	return (dst);
}

/** set the vec3i values */
t_vec3i * vec3i_set(t_vec3i * dst, int x, int y, int z) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = x;
	dst->y = y;
	dst->z = z;
	return (dst);
}

t_vec3i * vec3i_set3(t_vec3i * dst, t_vec3i * vec) {
	if (dst == vec) {
		return (dst);
	}
	return (vec3i_set(dst, vec->x, vec->y, vec->z));
}

/** add two vec3i */
t_vec3i * vec3i_add(t_vec3i * dst, t_vec3i * left, t_vec3i * right) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x + right->x;
	dst->y = left->y + right->y;
	dst->z = left->z + right->z;
	return (dst);
}

/** sub two vec3i */
t_vec3i * vec3i_sub(t_vec3i * dst, t_vec3i * left, t_vec3i * right) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x - right->x;
	dst->y = left->y - right->y;
	dst->z = left->z - right->z;
	return (dst);
}

/** mult the vec3i by the given scalar */
t_vec3i * vec3i_mult(t_vec3i * dst, t_vec3i * vec, int scalar) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = vec->x * scalar;
	dst->y = vec->y * scalar;
	dst->z = vec->z * scalar;
	return (dst);
}

t_vec3i * vec3i_mult3(t_vec3i * dst, t_vec3i * left, t_vec3i * right) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x * right->x;
	dst->y = left->y * right->y;
	dst->z = left->z * right->z;
	return (dst);
}

/** cross product */
t_vec3i * vec3i_cross(t_vec3i * dst, t_vec3i * left, t_vec3i * right) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	
	dst->x = left->y * right->z - left->z * right->y;
	dst->y = left->z * right->x - left->x * right->z;
	dst->z = left->x * right->y - left->y * right->x;
	return (dst);
}

/** scale product */
int vec3i_dot_product(t_vec3i * left, t_vec3i * right) {
	return (left->x * right->x + left->y * right->y + left->z * right->z);
}

/** length */
int vec3i_length_squared(t_vec3i * vec) {
	return (vec3i_dot_product(vec, vec));
}

int vec3i_length(t_vec3i * vec) {
	return ((int)sqrt(vec3i_length_squared(vec)));
}

/** normalize */
t_vec3i * vec3i_normalize(t_vec3i * dst, t_vec3i * vec) {

	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}

	int norm = 1 / vec3i_length(vec);
	dst->x = vec->x * norm;
	dst->y = vec->y * norm;
	dst->z = vec->z * norm;
	return (dst);
}

/** negate */
t_vec3i * vec3i_negate(t_vec3i * dst, t_vec3i * src) {
	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = -src->x;
	dst->y = -src->y;
	dst->z = -src->z;
	return (dst);
}

/** angle between two vec */
int vec3i_angle(t_vec3i * left, t_vec3i * right) {
	int dls = vec3i_dot_product(left, right) / (vec3i_length(left) * vec3i_length(right));
	if (dls < -1.0f) {
		dls = -1.0f;
	} else if (dls > 1.0f) {
		dls = 1.0f;
	}
	return ((int)acos(dls));
}

/** mix the two vectors */
t_vec3i * vec3i_mix(t_vec3i * dst, t_vec3i * left, t_vec3i * right, int ratio) {

	if (dst == NULL) {
		if ((dst = vec3i_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = left->x * ratio + right->x * (1 - ratio);
	dst->y = left->y * ratio + right->y * (1 - ratio);
	dst->z = left->z * ratio + right->z * (1 - ratio);
	return (dst);
}

/** comparison */
int vec3i_equals(t_vec3i * left, t_vec3i * right) {
	return (left == right || (left->x == right->x && left->y == right->y && left->z == right->z));
}

int vec3i_nequals(t_vec3i * left, t_vec3i * right) {
	return (!vec3i_equals(left, right));
}

/** hash */
int vec3i_hash(t_vec3i * vec) {
	return ((vec->x * 73856093) ^ (vec->y * 19349663) ^ (vec->z * 83492791));
}

/** to string: return a string allocated with malloc() */
char * vec3i_str(t_vec3i * vec) {
	if (vec == NULL) {
		return (strdup("vec3i(NULL)"));
	}
	char buffer[160];
	sprintf(buffer, "vec3i(%d ; %d ; %d)", vec->x, vec->y, vec->z);
	return (strdup(buffer));
}
