/**
 *  This file is part of https://github.com/toss-dev/C_maths
 *
 *  It is under a GNU GENERAL PUBLIC LICENSE
 *
 *  This library is still in development, so please, if you find any issue, let me know about it on github.com
 *  PEREIRA Romain
 */

#include "vec2f.h"

/** create a new vec2f */
t_vec2f * vec2f_new(void) {
	return ((t_vec2f *)malloc(sizeof(t_vec2f)));
}

/** delete the vec2f */
void vec2f_delete(t_vec2f * vec) {
	free(vec);
}

/** set the vec2f to 0 */
t_vec2f * vec2f_zero(t_vec2f * dst) {
	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}
	memset(dst, 0, sizeof(t_vec2f));
	return (dst);
}

/** set the vec2f values */
t_vec2f * vec2f_set(t_vec2f * dst, float x, float y) {
	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = x;
	dst->y = y;
	return (dst);
}

t_vec2f * vec2f_set2(t_vec2f * dst, t_vec2f * vec) {
	if (dst == vec) {
		return (dst);
	}
	return (vec2f_set(dst, vec->x, vec->y));
}

/** add two vec2f */
t_vec2f * vec2f_add(t_vec2f * dst, t_vec2f * left, t_vec2f * right) {
	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x + right->x;
	dst->y = left->y + right->y;
	return (dst);
}

/** sub two vec2f */
t_vec2f * vec2f_sub(t_vec2f * dst, t_vec2f * left, t_vec2f * right) {
	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x - right->x;
	dst->y = left->y - right->y;
	return (dst);
}

/** mult the vec2f by the given scalar */
t_vec2f * vec2f_mult(t_vec2f * dst, t_vec2f * vec, float scalar) {
	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = vec->x * scalar;
	dst->y = vec->y * scalar;
	return (dst);
}

t_vec2f * vec2f_mult2(t_vec2f * dst, t_vec2f * left, t_vec2f * right) {
	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x * right->x;
	dst->y = left->y * right->y;
	return (dst);
}

/** scale product */
float vec2f_dot_product(t_vec2f * left, t_vec2f * right) {
	return (left->x * right->x + left->y * right->y);
}

/** length */
float vec2f_length_squared(t_vec2f * vec) {
	return (vec2f_dot_product(vec, vec));
}

float vec2f_length(t_vec2f * vec) {
	return ((float)sqrt(vec2f_length_squared(vec)));
}

/** normalize */
t_vec2f * vec2f_normalize(t_vec2f * dst, t_vec2f * vec) {

	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}

	float norm = 1 / vec2f_length(vec);
	dst->x = vec->x * norm;
	dst->y = vec->y * norm;
	return (dst);
}

/** negate */
t_vec2f * vec2f_negate(t_vec2f * dst, t_vec2f * src) {
	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = -src->x;
	dst->y = -src->y;
	return (dst);
}

/** angle between two vec */
float vec2f_angle(t_vec2f * left, t_vec2f * right) {
	float dls = vec2f_dot_product(left, right) / (vec2f_length(left) * vec2f_length(right));
	if (dls < -1.0f) {
		dls = -1.0f;
	} else if (dls > 1.0f) {
		dls = 1.0f;
	}
	return ((float)acos(dls));
}

/** mix the two vectors */
t_vec2f * vec2f_mix(t_vec2f * dst, t_vec2f * left, t_vec2f * right, float ratio) {

	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = left->x * ratio + right->x * (1 - ratio);
	dst->y = left->y * ratio + right->y * (1 - ratio);
	return (dst);
}

/** comparison */
int vec2f_equals(t_vec2f * left, t_vec2f * right) {
	return (left == right || (left->x == right->x && left->y == right->y));
}

int vec2f_nequals(t_vec2f * left, t_vec2f * right) {
	return (!vec2f_equals(left, right));
}

/** hash */
int vec2f_hash(t_vec2f * vec) {
	return ((int)(vec->x * 73856093.0f) ^ (int)(vec->y * 19349663.0f));
}

/** round vec2f */
t_vec2f * vec2f_round(t_vec2f * dst, t_vec2f * vec, int decimals) {
	static float powten[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

	if (decimals < 0 || decimals >= 10) {
		return (vec2f_set2(dst, vec));
	}

	if (dst == NULL) {
		if ((dst = vec2f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = roundf(powten[decimals] * vec->x) / powten[decimals];
	dst->y = roundf(powten[decimals] * vec->y) / powten[decimals];
	return (dst);
}

/** to string: return a string allocated with malloc() */
char * vec2f_str(t_vec2f * vec) {
	if (vec == NULL) {
		return (strdup("vec2f(NULL)"));
	}
	char buffer[128];
	sprintf(buffer, "vec2f(%f ; %f)", vec->x, vec->y);
	return (strdup(buffer));
}
