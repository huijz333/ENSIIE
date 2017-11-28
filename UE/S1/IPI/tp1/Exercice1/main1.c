#include <stdio.h>

/** swap two given integers using arithmetic instead of a third variable */
static void swap_2(int * a, int * b) {
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

/** swap two integers using a third variable */
static void swap_1(int * a, int * b) {
	int c = *a;
	*a = *b;
	*b = c;
}

int main(void) {
	int a, b;

	a = 42;
	b = 24;
	printf("a=%d and b=%d\n", a, b);
	printf("Swapping...\n");
	swap_1(&a, &b);
	printf("a=%d and b=%d\n", a, b);
	printf("Swapping...\n");
	swap_2(&a, &b);
	printf("a=%d and b=%d\n", a, b);
	return (0);
}
