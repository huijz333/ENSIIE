#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/** allocation */
static double * q1(double a, double b){
	double * r = (double *)malloc(sizeof(double));
	if (r != NULL) {
		*r = a < b ? b : a;
	}
	return (r);
}

static int * q2(int * a, int * b, int size) {
	int * r = (int*)malloc(sizeof(int) * size);
	if (r == NULL) {
		return (NULL);
	}
	int i;
	for (i = 0 ; i < size ; i++) {
		r[i] = b[i] - a[i];
	}
	return (r);
}
	
static void q3_helper(int * array, int size, int * r) {
	char * visited = (char*)malloc(sizeof(char) * size);
	if (visited == NULL) {
		return ;
	}
	memset(visited, 0, sizeof(char) * size);
	
	int i = 0;
	int j;
	for (j = 0 ; j < size ; j++) {
		int k = 0;
		int l;
		for (l = 0 ; l < size ; l++) {
			if (array[l] < array[k] && !visited[l]) {
				k = l;
			}
		}
		r[i++] = array[k];
		visited[k] = 1;
	}
	free(visited);
}

static int * q3(int * array, int size) {
	int * r = (int*)malloc(sizeof(int) * size);
	if (r == NULL) {
		return (NULL);
	}
	q3_helper(array, size, r);
	return (r);
}

static int * q4(int * ta, int * tb, int n) {
	int * diff = q2(ta, tb, n);
	if (diff == NULL) {
		return (NULL);
	}
	int i;
	int j;
	for (j = 0 ; j < n ; j++) {
		if (diff[j] == 0) {
			++i;
		}
	}

	int * r = (int*)malloc(sizeof(int) * i);
	if (r == NULL) {
		free(diff);
		return (NULL);
	}

	i = 0;
	for (j = 0 ; j < n ; j++) {
		if (diff[j] == 0) {
			r[i] = ta[j];
			int k = i;
			while (k > 0 && r[k - 1] > r[k]) {
				int tmp = r[k-1];
				r[k-1] = r[k];
				r[k] = tmp;
				--k;
			}
			++i;
		}
	}

	free(diff);
	return (r);
}

int main() {
	t_list * lst = list_new();

	list_delete(lst);
	return (0);
}
