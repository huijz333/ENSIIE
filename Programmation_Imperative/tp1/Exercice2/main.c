#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	if (argc != 3) {
		printf("error usage: %s width height\n", argv[0]);
		return (0);
	}
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);

	int x, y;
	for (y = 0 ; y < height ; y++) {
		for (x = 0 ; x < width ; x++) {
			printf("*");
		}
		printf("\n");
	}

	return (0);
}
