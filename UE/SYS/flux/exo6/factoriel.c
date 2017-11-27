#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

// renvoie 1 si str est un nombre en base 10
// renvoie 0 dans le cas contraire 
int isNumber(const char* str)
{
	for ( ; *str ; str+=1) 
		if ( *str<'0' || '9'<*str )
			return 0;
	return 1;
}

char tmp[300];
char* envv_next[4] = {tmp, tmp+100, tmp+200, 0};
char* argv_next[1] = {0};

int main(int argc, char*argv[], char* envv[]) {

	//if too many arguments, error
	if (argc != 0 && argc != 1) {
		fprintf(stderr,"%s:usage: %s\n", argv[0],argv[0]);
		return (EXIT_FAILURE);
	}

	//recursivity, if  no arguments... calculate (n-1)!
	char * cmd;
	if (argc == 0) {
		// exécutions suivantes
		int n     = atoi(envv[0]);
		int fac   = atoi(envv[1]);
		cmd = envv[2];
		if (n == 1) {
			printf("factoriel de %s = %d\n",basename(cmd),fac);
			return 0;
		}
		sprintf(envv_next[0], "%d", n - 1);
		sprintf(envv_next[1], "%d", fac * n);
		envv_next[2] = cmd;
	} else {
	
		// if argument, this is the first call, check that n is valid, and setup
		if (!isNumber(basename(argv[0])) ) {
			fprintf(stderr,"%s:usage: number\n", argv[0]);
			return (EXIT_FAILURE);
		}
		envv_next[0] = basename(argv[0]);
		envv_next[1] = "1";
		envv_next[2] = argv[0];
		cmd = argv[0];
	}
	char ** arg = {NULL};
	execve(cmd, arg, envv_next);
	fprintf(stderr, "execve(%s...) : %s\n", envv_next[2],strerror(errno));

	return (EXIT_SUCCESS);
}

