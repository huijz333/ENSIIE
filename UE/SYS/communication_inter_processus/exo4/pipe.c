#include <stdio.h> //print
#include <unistd.h> //write, pipe
#include <errno.h>

int main() {
	int fds[2];
	if (pipe(fds) == -1) {
		fprintf(stderr, "pipe(fds) error: %s\n", strerror(errno));
	}
	
	unsigned long int size = 0;
	char c = 'a';
	int r = 0;
	while (1) {
		printf("%d bytes written into pipe : %d\n", size, r);
		if (write(fds[1], &c, 1) == -1) {
			break ;
		}
		++size;
	}
	return (0);
}
