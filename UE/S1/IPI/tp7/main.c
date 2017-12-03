#include <stdio.h>


# define T (int) //matrix type

typedef matrix T **; //typedef

matrix matrix_new(size_t m) {
	//allocate memory with one malloc
	matrix mat = (matrix) malloc(sizeof(T *) * m + sizeof(T) * m);
	if (mat == NULL) {
		return (NULL);
	}

	//place memory
	size_t i;
	for (i = 0 ; i < m ; i++) {
		mat[i] = mat;
	}

	return (mat);
}

int main(void) {

	return (0);
}
