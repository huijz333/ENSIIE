#include "/pub/ia/sys/syscall/lire.h"
#include <unistd.h> //execlp

int main() {
	lire();
	lire();
	execlp("./lire", "", NULL);
	return 0 ;
}
