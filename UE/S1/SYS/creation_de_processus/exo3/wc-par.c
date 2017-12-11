# include <stdio.h> // printf
# include <unistd.h> //fork(), pipe()
# include <stdlib.h> //exit()
# include <errno.h>
# include <string.h>
# include <sys/types.h> //waitpid()
# include <sys/wait.h> //waitpid()
# include <sys/stat.h> //stat()

# define N_PROCESS (4)
/**
	cette fonction calcul le nombre de ligne du fichier 'filepath',
	de taille total 'sz',
	et lis les octets de [i * sz / N_PROCESS, (i + 1) * sz / N_PROCESS)]
*/
static unsigned int countLines(char * filepath, size_t sz, unsigned int i) {
	/** on ouvre le fichier */
	FILE * f = fopen(filepath, "r");
	if (f == NULL) {
		return (-1);
	}
	/** calcul des dimensions */
	size_t begin = sz / N_PROCESS * i;
	size_t end = sz / N_PROCESS * (i + 1);
	size_t len = (end - begin) * sizeof(char);
	char * buffer = (char *)malloc(len);
	if (buffer == NULL) {
		fclose(f);
		return (-1);
	}
	/** on bouge le curseur du fichier au debut */
	fseek(f, begin, SEEK_SET);
	/** on lit les octets */
	if (fread(buffer, len, 1, f) != 1) {
		free(buffer);
		fclose(f);
		fprintf(stderr, "error on fread()\n");	
		return (-1);
	}
	/** plus besoin du fichier , on le ferme */
	fclose(f);

	/** on compte les occurences du caracteres dans les octets lus */
	unsigned int occ = 0;
	unsigned int j;
	for (j = 0 ; j < len; j++) {
		if (buffer[j] == '\n') {
			++occ;
		}
	}
	/** on n'oublie pas de free le buffer */
	free(buffer);
	/** done */
	return (occ);
}

int main(int argc, char ** argv) {
	if (argc <= 1) {
		fprintf(stderr, "error usage: %s f1 f2 ... fi\n", argv[0]);
		return (EXIT_FAILURE);
	}

	/** on recupere taille du fichier */
	char * filepath = argv[1];
	struct stat st;
	stat(filepath, &st);
	size_t sz = st.st_size;

	/**
		on va creer autant de pipe qu'il y a de processus.
	*/
	int fds[2];
	if (pipe(fds) == -1) {
		exit(EXIT_FAILURE);
	}
	unsigned int i;
	for (i = 0 ; i < N_PROCESS ; i++) {
		/** on cree le pipe */
		/** on cree le processus */
		pid_t pid = fork();
		if (pid == -1) {
			fprintf(stderr, "too much processes! %d\n", i);
			exit(EXIT_FAILURE);
		}
		/** si on est dans le child */
		if (pid == 0) {
			/** on compte le nombre de ligne */
			unsigned int count = countLines(filepath, sz, i);
			/** on l'ecrit dans le pipe */
			write(fds[1], &count, sizeof(unsigned int));
			/** on stop le child */
			exit(EXIT_SUCCESS);
		}
	}

	/** seul le parent arrive ici: on attends tous les childs */
	int wstatut;
	while (wait(&wstatut) > 0);

	/** on lit chaque pipe, et on somme */
	unsigned int total = 0;
	for (i = 0 ; i < N_PROCESS ; i++) {
		unsigned int count;
		read(fds[0], &count, sizeof(unsigned int));
		total += count;
	}
	close(fds[0]);
	/** on affiche le resultat */
	printf("runned with %d processes\n", N_PROCESS);
	printf("total line: %d\n", total);
	return (EXIT_SUCCESS);
}
