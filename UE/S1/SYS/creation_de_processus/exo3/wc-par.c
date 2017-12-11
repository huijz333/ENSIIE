# include <stdio.h> // printf
# include <unistd.h> //fork()
# include <stdlib.h> //exit()
# include <errno.h>
# include <string.h>
# include <sys/types.h> //waitpid()
# include <sys/wait.h> //waitpid()
# include <sys/stat.h> //stat()

# define N_PROCESS (4)

static size_t countLines(char * filepath, size_t sz, int i) {
	FILE * f = fopen(filepath, "r");
	if (f == NULL) {
		return (-1);
	}
	size_t begin = sz / N_PROCESS * i;
	size_t end = sz / N_PROCESS * (i + 1);
	size_t len = end - begin;
	char * buffer = (char *)malloc(len * sizeof(char));
	if (buffer == NULL) {
		fclose(f);
		return (-1);
	}
	fseek(f, begin, SEEK_SET);
	printf("cursor is at: %lu\n", ftell(f));
	size_t r = fread(buffer, len, sizeof(char), f);
	free(buffer);
	fclose(f);
	if (r != len) {
		printf("read %lu instead of %lu exepected bytes\n", r, len);
		return (-1);
	}

	size_t occ = 0;
	size_t j;
	for (j = begin ; j < end; j++) {
		if (buffer[j] == '\n') {
			++occ;
		}
	}

	return (occ);
}

int main(int argc, char ** argv) {
	if (argc <= 1) {
		fprintf(stderr, "error usage: %s f1 f2 ... fi\n", argv[0]);
		return (EXIT_FAILURE);
	}

	char * filepath = argv[1];

	struct stat st;
	stat(filepath, &st);
	size_t sz = st.st_size;
	printf("filesize: %lu\n", sz);

	int i;
	for (i = 0 ; i < N_PROCESS ; i++) {
		size_t occ = countLines(filepath, sz, i);
		printf("%lu\n", occ);
	}
	
	return (0);
}
