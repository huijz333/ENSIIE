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

static void sig_ignore(int signum) {

}

static void sig_usr1(int signum) {
	fprintf(stderr, "fils:%d: fin due à la reception du signal SIGUSR1\n", getpid());
	exit(EXIT_FAILURE);
}

void fils(char* nickname, int in)
{
	signal(SIGINT, sig_ignore);
	signal(SIGUSR1, sig_usr1);
	int x;
	int len=sizeof x;
	while (1) {
		if (read(in,&x,len)!=len) continue;
		printf("%s:%d: %d\n",nickname,getpid(),x);
		if (x==0) break;
	}
	exit(0);
}

static void sig_int(int signum) {
	fprintf(stderr, "\n");
	kill(pid_pos, SIGUSR1);
	kill(pid_neg, SIGUSR1);
	int r;
	while (wait(&r) > 0);
	printf("pere:%d: mes fils sont morts (fin)\n", getpid());
	exit(0);

}

void pere(char* nickname, int pos[2], int neg[2])
{
	signal(SIGINT, sig_int);
	int x;
	int len=sizeof x;
	dup2(2,1);
	while (1) {
		printf("entrez un entier: ");
		if ( scanf("%d",&x)!=1 ) {
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
	if ( (pid_pos=fork())==0 ) { 
		fils("filsP",pos[0]);
	} else if ( (pid_neg=fork())==0 ) {
		fils("filsN",neg[0]);
	} else {
		pere("pere",pos,neg);
	}

	fprintf(stderr,"Argh!!!\n");
	return 255;
}
