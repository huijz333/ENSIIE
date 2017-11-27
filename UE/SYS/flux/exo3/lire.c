#include "/pub/ia/sys/syscall/lire.h"
#include <stdio.h>

int main(void) {
	fprintf(stderr, "je suis la fonction lire\n");
	lire();
	return (0);
}
