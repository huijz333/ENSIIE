# include <stdio.h> // printf
# include <unistd.h> //fork()
# include <stdlib.h> //exit()
# include <errno.h>
# include <string.h>
# include <sys/types.h> //waitpid()
# include <sys/wait.h> //waitpid()

typedef int (* t_cmpf)(void const *, void const *);

/** man qsort */
static int intcmp(int * a, int * b) {
	return (*a - *b);
}

int main(int argc, char ** argv) {
	if (argc <= 1) {
		fprintf(stderr, "error usage: %s n1 n2 ... ni\n", argv[0]);
		return (EXIT_FAILURE);
	}

	//parsing integers
	int size = argc - 1;
	int n[size];
	int i;
	for (i = 0 ; i < size ; i++) {
		n[i] = atoi(argv[i + 1]);
	}
	//sorting integers
	qsort(n, size, sizeof(int), (t_cmpf) intcmp);
	
	for (i = 0 ; i < size ; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			sleep(n[i]);
			printf("%d\n", n[i]);
			exit(0);
		}
	}

	//wait for each childs
	int wstatut;
	while (wait(&wstatut) > 0);
	
	return (0);
}
