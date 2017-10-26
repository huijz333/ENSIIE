#include "main.h"

static void printArray1(int * array, int n) {
	int i;
	for (i = 0 ; i < n ; i++) {
		printf("%d     ", array[i]);
	}
	printf("\n");
}

static void printArray2(int * array, int n) {
	int i;
	for (i = 0 ; i < n ; i++) {
		printf("[%d] : %d\n", i, array[i]);
	}
}

static double averageArray(int * array, int n) {
	int sum = 0;
	int i;
	for (i = 0 ; i < n ; i++) {
		sum += array[i];
	}
	return (sum / (double)n);
}

static double polynomeValue(int * P, int n, double x) {
	int i;
	double y = 0;
	for (i = 0 ; i < n ; i++) {
		y += P[i] * pow(x, i);
	}
	return (y);
}

static void polynomeSum(int * p1, int * p2, int *p3, int n) {
	int i;
	for (i = 0 ; i < n; i++) {
		p3[i] = p1[i] + p2[i];
	}
}

static int contains(int * array, int n, int value) {
	int i;
	for (i = 0 ; i < n ; i++) {
		if (array[i] == value) {
			return (1);
		}
	}
	return (0);

}

static int isReferenced(int * ref, int n, int value) {
	return (contains(ref, n, value));
}

static int hasPrice(int * prices, int n, int value) {
	return (contains(prices, n, value));
}

static void printReferences(int * ref, int * prices, int n, int price) {
	int i;
	for (i = 0 ; i < n ; i++) {
		if (prices[i] == price) {
			printf("%d ", ref[i]);
		}
	}
	printf("\n");
}

static void selectedSort(int * array, int n) {
	int i, j, min;
	for (i = 0 ; i < n ; i++) {
		min = i;
		for (j = i + 1 ; j < n ; j++) {
			if (array[j] < array[min]) {
				min = j;
			}
		}
		if (min != i) {
			int tmp = array[i];
			array[i] = array[min];
			array[min] = tmp;
		}
	}
}

static void bubbleSort(int * array, int n) { 
	int i, j;
	for (i = n - 2 ; i >= 0 ; i--) {
		for (j = 0 ; j <= i ; j++) {
			printf("%d\n", j);
			if (array[j + 1] < array[j]) {
				int tmp = array[j + 1];
				array[j + 1] = array[j];
				array[j] = tmp;
			}
		}
	}
}

int main() {
	//TABLEAU
	{
		int n = 5;
		int array[] = {42, 41, 40, 39, 38};
		printArray1(array, n);
		printArray2(array, n);
		printf("%f\n", averageArray(array, n));
	}
	//POLYNOMES
	{
		int N = 8;
		int P[] = {42, 12, 0, 0, 0, 3, 0, 1};
		double x = 0.5;
		printf("\nPolynome:\n");
		printArray2(P, N);
		printf("P(%f)=%f\n", x, polynomeValue(P, N, x));
	}
	//tables
	{
		printf("\n");
		int n = 4;
		int ref[] = {1, 2, 3, 4};
		int prices[] = {10, 20, 30, 10};
		printf("%d : %d\n", isReferenced(ref, n, 4), hasPrice(prices, n, 10));
		printf("%d : %d\n", isReferenced(ref, n, 42), hasPrice(prices, n, 42));
		printReferences(ref, prices, n, 10);
	}
	//tri
	{
		printf("\n");
		int n = 8;
		int array1[] = {42, 1, 32, 4, 5, 8, 789, 1};
		selectedSort(array1, n);
		printArray1(array1, n);

		int array2[] = {42, 1, 32, 4, 5, 8, 789, 1};
		bubbleSort(array2, n);
		printArray1(array2, n);
	}
	return (0);
}
