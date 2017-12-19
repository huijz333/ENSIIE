/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#include "vec3f.h"

/** create a new vec3f */
t_vec3f * vec3f_new(void) {
	return ((t_vec3f *)malloc(sizeof(t_vec3f)));
}

/** delete the vec3f */
void vec3f_delete(t_vec3f * vec) {
	free(vec);
}

/** set the vec3f to 0 */
t_vec3f * vec3f_zero(t_vec3f * dst) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	memset(dst, 0, sizeof(t_vec3f));
	return (dst);
}

/** set the vec3f values */
t_vec3f * vec3f_set(t_vec3f * dst, float x, float y, float z) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = x;
	dst->y = y;
	dst->z = z;
	return (dst);
}

t_vec3f * vec3f_set3(t_vec3f * dst, t_vec3f * vec) {
	if (dst == vec) {
		return (dst);
	}
	return (vec3f_set(dst, vec->x, vec->y, vec->z));
}

/** add two vec3f */
t_vec3f * vec3f_add(t_vec3f * dst, t_vec3f * left, t_vec3f * right) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x + right->x;
	dst->y = left->y + right->y;
	dst->z = left->z + right->z;
	return (dst);
}

/** sub two vec3f */
t_vec3f * vec3f_sub(t_vec3f * dst, t_vec3f * left, t_vec3f * right) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x - right->x;
	dst->y = left->y - right->y;
	dst->z = left->z - right->z;
	return (dst);
}

/** mult the vec3f by the given scalar */
t_vec3f * vec3f_mult(t_vec3f * dst, t_vec3f * vec, float scalar) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = vec->x * scalar;
	dst->y = vec->y * scalar;
	dst->z = vec->z * scalar;
	return (dst);
}

t_vec3f * vec3f_mult3(t_vec3f * dst, t_vec3f * left, t_vec3f * right) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x * right->x;
	dst->y = left->y * right->y;
	dst->z = left->z * right->z;
	return (dst);
}

/** cross product */
t_vec3f * vec3f_cross(t_vec3f * dst, t_vec3f * left, t_vec3f * right) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	
	dst->x = left->y * right->z - left->z * right->y;
	dst->y = left->z * right->x - left->x * right->z;
	dst->z = left->x * right->y - left->y * right->x;
	return (dst);
}

/** scale product */
float vec3f_dot_product(t_vec3f * left, t_vec3f * right) {
	return (left->x * right->x + left->y * right->y + left->z * right->z);
}

/** length */
float vec3f_length_squared(t_vec3f * vec) {
	return (vec3f_dot_product(vec, vec));
}

float vec3f_length(t_vec3f * vec) {
	return ((float)sqrt(vec3f_length_squared(vec)));
}

/** normalize */
t_vec3f * vec3f_normalize(t_vec3f * dst, t_vec3f * vec) {

	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}

	float norm = 1 / vec3f_length(vec);
	dst->x = vec->x * norm;
	dst->y = vec->y * norm;
	dst->z = vec->z * norm;
	return (dst);
}

/** negate */
t_vec3f * vec3f_negate(t_vec3f * dst, t_vec3f * src) {
	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = -src->x;
	dst->y = -src->y;
	dst->z = -src->z;
	return (dst);
}

/** angle between two vec */
float vec3f_angle(t_vec3f * left, t_vec3f * right) {
	float dls = vec3f_dot_product(left, right) / (vec3f_length(left) * vec3f_length(right));
	if (dls < -1.0f) {
		dls = -1.0f;
	} else if (dls > 1.0f) {
		dls = 1.0f;
	}
	return ((float)acos(dls));
}

/** mix the two vectors */
t_vec3f * vec3f_mix(t_vec3f * dst, t_vec3f * left, t_vec3f * right, float ratio) {

	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = left->x * ratio + right->x * (1 - ratio);
	dst->y = left->y * ratio + right->y * (1 - ratio);
	dst->z = left->z * ratio + right->z * (1 - ratio);
	return (dst);
}

/** comparison */
int vec3f_equals(t_vec3f * left, t_vec3f * right) {
	return (left == right || (left->x == right->x && left->y == right->y && left->z == right->z));
}

int vec3f_nequals(t_vec3f * left, t_vec3f * right) {
	return (!vec3f_equals(left, right));
}

/** hash */
int vec3f_hash(t_vec3f * vec) {
	return ((int)(vec->x * 73856093.0f) ^ (int)(vec->y * 19349663.0f) ^ (int)(vec->z * 83492791.0f));
}

/** round vec3f */
t_vec3f * vec3f_round(t_vec3f * dst, t_vec3f * vec, int decimals) {
	static float powten[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

	if (decimals < 0 || decimals >= 10) {
		return (vec3f_set3(dst, vec));
	}

	if (dst == NULL) {
		if ((dst = vec3f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = roundf(powten[decimals] * vec->x) / powten[decimals];
	dst->y = roundf(powten[decimals] * vec->y) / powten[decimals];
	dst->z = roundf(powten[decimals] * vec->z) / powten[decimals];
	return (dst);
}

/** to string: return a string allocated with malloc() */
char * vec3f_str(t_vec3f * vec) {
	if (vec == NULL) {
		return (strdup("vec3f(NULL)"));
	}
	char buffer[160];
	sprintf(buffer, "vec3f(%f ; %f ; %f)", vec->x, vec->y, vec->z);
	return (strdup(buffer));
}
