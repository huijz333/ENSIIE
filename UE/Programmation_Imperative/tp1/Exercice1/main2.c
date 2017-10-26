#include <stdio.h>
#include <math.h>

int main(void) {
	printf("pow(0, -100) == %f\n", pow(0, -100));
	printf("pow(-4, 0.5) == %f\n", pow(-4, 0.5));
	printf("PI approximation using tangeante: %f\n", atan(999999999999.0) * 2.0);
	return (0);
}
