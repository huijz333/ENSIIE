#include "lire.h"
#include <unistd.h> //execlp

int main()
{
	fprintf(stderr, "je suis la commande lire()\n");
	lire();
	lire();
	execlp("./lire", "/pub/ia/sys/syscall/data.in");
	return 0 ;
}
