# include <stdio.h>
# include <stdlib.h> //EXIT_FAILURE
# include "mysleep.h"

int main(int argc, char ** argv) {
	if (argc != 2) {
		fprintf(stderr, "%s: usage %s [SEC]\n", argv[0], argv[0]);
		return (EXIT_FAILURE);
	}
	mysleep(atoi(argv[1]));
	return (0);
}
