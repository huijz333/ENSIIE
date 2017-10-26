#include <stdio.h>

int pow1(int a, int b) {
	int r = 1;
	while (b > 0) {
		r *= a;
		--b;
	}
	return (r);
}

int pow2(int x, int p) {
	return ( (x == 1 || p == 0) ? 1 : (p == 2) ? x * x : (p % 2 == 0) ? pow2(pow2(x, p / 2), 2) : x * pow2(pow2(x, (p - 1) / 2), 2));
}

int main() {
	int x = 3;
	int p = 1000000000;
	printf("pow1(%d, %d) == %d\n", x, p, pow1(x, p));
	printf("pow2(%d, %d) == %d\n", x, p, pow2(x, p));
	return (0);
}
