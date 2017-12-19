# include "matrix.h"

static void matrix_pascal(t_matrix * mat) {
	size_t i, j;
	matrix_zeros(mat);
	matrix_set(mat, 0, 0, 1);
	for (i = 1 ; i < mat->n ; i++) {
		matrix_set(mat, i, 0, 1);
		matrix_set(mat, i, i, 1);
		for (j = 1 ; j < i ; j++) {
			int top = matrix_get(mat, i - 1, j);
			int topLeft = matrix_get(mat, i - 1, j - 1);
			matrix_set(mat, i, j, top + topLeft);
		}
	}

}

/** see matrix_print() */
static void printInteger(t_matrix * matrix, size_t i, size_t j) {
	printf("%d", matrix_get(matrix, i, j));
}

/** triangle de pascal */
int main(void) {
	size_t n = 15;

	t_matrix * mat = matrix_new(n, n);
	matrix_pascal(mat);
	matrix_print(mat, printInteger);
	matrix_delete(mat);
	return (0);
}
