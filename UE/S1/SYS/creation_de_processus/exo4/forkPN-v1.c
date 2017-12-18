#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// attention: c'est pas un header système
#include "waitverbose.h"

int pid_pos,pid_neg;
int pos[2];
int neg[2];

void fils(char* nickname, int in)
{
	int x;
	int len=sizeof x;
	while (1) {
		/** ici */
		int r = read(in,&x,len);
		if (r <= 0) {
			break ;
		} else if (r != len) {
			continue ;
		}
		printf("%s:%d: %d\n",nickname,getpid(),x);
		if (x==0) break;
	}
	exit(0);
}

void pere(char* nickname, int pos[2], int neg[2])
{
	int x;
	int len=sizeof x;
	dup2(2,1);
	while (1) {
		printf("entrez un entier: ");
		/* ici */
		int r = scanf("%d",&x);
		if (r == EOF) {
			close(pos[1]);
			close(neg[1]);
			break ;
		} else if ( r!=1 ) {
			fprintf(stderr,"%s:%d: probleme lecture stdin.\n",
					nickname,getpid());
			while ( getchar()!='\n' );
			continue;
		}
		if ( x>=0 ) write(pos[1],&x,len);
		if ( x<=0 ) write(neg[1],&x,len);
		if ( x==0 ) break;
	}
	waitendverbose(0);
	waitendverbose(0);
	exit(0);
}

int main(int argc, char*argv[])
{
	if ( pipe(pos)<0 || pipe(neg) ) {
		fprintf(stderr,"%s: probleme creation pipe\n",argv[0]);
		exit (1);
	}
	/* ici */
	if ( (pid_pos=fork())==0 ) { 
		close(pos[1]);
		close(neg[1]);
		fils("filsP",pos[0]);
	}
	else if ( (pid_neg=fork())==0 ) {
		close(pos[1]);
		close(neg[1]);
		fils("filsN",neg[0]);
	} else {
		close(pos[0]);
		close(neg[0]);
		pere("pere",pos,neg);
	}
	fprintf(stderr,"Argh!!!\n");
	return 255;
}
