/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#include "vec4f.h"

/** create a new vec4f */
t_vec4f * vec4f_new(void) {
	return ((t_vec4f *)malloc(sizeof(t_vec4f)));
}

/** delete the vec4f */
void vec4f_delete(t_vec4f * vec) {
	free(vec);
}

/** set the vec4f to 0 */
t_vec4f * vec4f_zero(t_vec4f * dst) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}
	memset(dst, 0, sizeof(t_vec4f));
	return (dst);
}

/** set the vec4f values */
t_vec4f * vec4f_set(t_vec4f * dst, float x, float y, float z, float w) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = x;
	dst->y = y;
	dst->z = z;
	dst->w = w;
	return (dst);
}

t_vec4f * vec4f_set4(t_vec4f * dst, t_vec4f * vec) {
	if (dst == vec) {
		return (dst);
	}
	return (vec4f_set(dst, vec->x, vec->y, vec->z, vec->w));
}

/** add two vec4f */
t_vec4f * vec4f_add(t_vec4f * dst, t_vec4f * left, t_vec4f * right) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x + right->x;
	dst->y = left->y + right->y;
	dst->z = left->z + right->z;
	dst->w = left->w + right->w;
	return (dst);
}

/** sub two vec4f */
t_vec4f * vec4f_sub(t_vec4f * dst, t_vec4f * left, t_vec4f * right) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x - right->x;
	dst->y = left->y - right->y;
	dst->z = left->z - right->z;
	dst->w = left->w - right->w;
	return (dst);
}

/** mult the vec4f by the given scalar */
t_vec4f * vec4f_mult(t_vec4f * dst, t_vec4f * vec, float scalar) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = vec->x * scalar;
	dst->y = vec->y * scalar;
	dst->z = vec->z * scalar;
	dst->w = vec->w * scalar;
	return (dst);
}

t_vec4f * vec4f_mult3(t_vec4f * dst, t_vec4f * left, t_vec4f * right) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
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
float vec4f_dot_product(t_vec4f * left, t_vec4f * right) {
	return (left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w);
}

/** length */
float vec4f_length_squared(t_vec4f * vec) {
	return (vec4f_dot_product(vec, vec));
}

float vec4f_length(t_vec4f * vec) {
	return ((float)sqrt(vec4f_length_squared(vec)));
}

/** normalize */
t_vec4f * vec4f_normalize(t_vec4f * dst, t_vec4f * vec) {

	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}

	float norm = 1 / vec4f_length(vec);
	dst->x = vec->x * norm;
	dst->y = vec->y * norm;
	dst->z = vec->z * norm;
	dst->w = vec->w * norm;
	return (dst);
}


/** negate */
t_vec4f * vec4f_negate(t_vec4f * dst, t_vec4f * src) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
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
float vec4f_angle(t_vec4f * left, t_vec4f * right) {
	float dls = vec4f_dot_product(left, right) / (vec4f_length(left) * vec4f_length(right));
	if (dls < -1.0f) {
		dls = -1.0f;
	} else if (dls > 1.0f) {
		dls = 1.0f;
	}
	return ((float)acos(dls));
}

/** mix the two vectors */
t_vec4f * vec4f_mix(t_vec4f * dst, t_vec4f * left, t_vec4f * right, float ratio) {

	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
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
int vec4f_equals(t_vec4f * left, t_vec4f * right) {
	return (left == right || (left->x == right->x && left->y == right->y && left->z == right->z && left->w == right->w));
}

int vec4f_nequals(t_vec4f * left, t_vec4f * right) {
	return (!vec4f_equals(left, right));
}

/** hash */
int vec4f_hash(t_vec4f * vec) {
	return ((int)(vec->x * 73856093.0f) ^ (int)(vec->y * 19349663.0f) ^ (int)(vec->z * 83492791.0f) ^ (int)(vec->w * 3539857.0f));
}

/** round vec4f */
t_vec4f * vec4f_round(t_vec4f * dst, t_vec4f * vec, int decimals) {
	static float powten[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

	if (decimals < 0 || decimals >= 10) {
		return (vec4f_set4(dst, vec));
	}

	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = roundf(powten[decimals] * vec->x) / powten[decimals];
	dst->y = roundf(powten[decimals] * vec->y) / powten[decimals];
	dst->z = roundf(powten[decimals] * vec->z) / powten[decimals];
	dst->w = roundf(powten[decimals] * vec->w) / powten[decimals];
	return (dst);
}

/** to string: return a string allocated with malloc() */
char * vec4f_str(t_vec4f * vec) {
	if (vec == NULL) {
		return (strdup("vec4f(NULL)"));
	}
	char buffer[256];
	sprintf(buffer, "vec4f(%f ; %f ; %f ; %f)", vec->x, vec->y, vec->z, vec->w);
	return (strdup(buffer));
}
