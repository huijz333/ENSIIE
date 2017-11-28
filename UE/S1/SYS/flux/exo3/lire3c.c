#include "/pub/ia/sys/syscall/lire.h"
#include <unistd.h> //execlp
#include <fcntl.h> //open
#include <errno.h>

int main(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(stderr, "error  usage: %s [IN] [OUT]\n", argv[0]);
		return (EXIT_FAILURE);
	}


	lire();
	lire();

	int in = open(argv[1], O_RDONLY);
	if (in == -1) {
		fprintf(stderr, "error with input file: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	}

	int out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 00644);
	if (out == -1) {
		fprintf(stderr, "error with output file: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	if (dup2(in, 0) == -1) {
		fprintf(stderr, "error while dup2(in): %s\n", strerror(errno));
		close(in);
		close(out);
		return (EXIT_FAILURE);
	}

	if (dup2(out, 1) == -1) {
		fprintf(stderr, "error while dup2(out): %s\n", strerror(errno));
		close(in);
		close(out);
		return (EXIT_FAILURE);
	}

	execlp("./lire", "", NULL);

	return (EXIT_SUCCESS);
}
