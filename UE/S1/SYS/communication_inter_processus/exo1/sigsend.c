#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

const char* progname;

const char* signal_itoa(int signum);
int         signal_atoi(const char* define);
void        signal_print();
int         isnumber(const char* p);

int main(int argc, char*argv[])
{
	progname = argv[0];
	int signum=-1,pid;

	// verification du signal 
	if (argc!=3) {
		fprintf(stderr,"%s:usage: %s signum pid\n",argv[0],argv[0]);
		fprintf(stderr,"%s:usage: %s sig    pid\n",argv[0],argv[0]);
		fprintf(stderr,"%s:usage: %s SIGsig pid\n",argv[0],argv[0]);
		exit (1);
	}
	// verification du signal 
	if ( isnumber(argv[1]) )
		signum = atoi(argv[1]);
	else
		signum = signal_atoi(argv[1]);
	if ( signum<0 || 22<signum ) {
		fprintf(stderr,"%s: %s est un signal invalide\n",argv[0],argv[1]);
		exit (1);
	}
	// verification du pid 
	if ( isnumber(argv[2]) )
		pid = atoi(argv[2]);
	if ( pid<2 ) {
		fprintf(stderr,"%s: %s est un pid invalide\n",argv[0],argv[2]);
		exit (1);
	}

	// envoie le signal signum au processus pid
	int status = kill(pid, signum); 
	if ( status==-1 ) {
		fprintf(stderr,"%s: échec de l'envoi de %s (%d) à %d: %s \n",
				argv[0],signal_itoa(signum),signum, pid,
				strerror(errno) );
		exit (1);
	}

	return 0;
}

const char* signal_itoa(int signum)
{
	switch (signum) {
		case SIGHUP : return "SIGHUP ";
		case SIGINT : return "SIGINT ";
		case SIGQUIT: return "SIGQUIT";
		case SIGILL : return "SIGILL ";
		case SIGABRT: return "SIGABRT";
		case SIGFPE : return "SIGFPE ";
		case SIGKILL: return "SIGKILL";
		case SIGSEGV: return "SIGSEGV";
		case SIGPIPE: return "SIGPIPE";
		case SIGALRM: return "SIGALRM";
		case SIGTERM: return "SIGTERM";
		case SIGUSR1: return "SIGUSR1";
		case SIGUSR2: return "SIGUSR2";
		case SIGCHLD: return "SIGCHLD";
		case SIGCONT: return "SIGCONT";
		case SIGSTOP: return "SIGSTOP";
		case SIGTSTP: return "SIGTSTP";
		case SIGTTIN: return "SIGTTIN";
		case SIGTTOU: return "SIGTTOU";
		default     : return "???????";
	}
}

int signal_atoi(const char* define)
{
	char sigstr[100],*p;
	strcpy(sigstr,define);
	for (p=sigstr ; *p ; p++) 
		if ( 'a'<=*p && *p<='z') *p += 'A' - 'a';

	if ( strstr("HUP", sigstr)!=0) return SIGHUP ;
	if ( strstr("INT", sigstr)!=0) return SIGINT ;
	if ( strstr("QUI", sigstr)!=0) return SIGQUIT;
	if ( strstr("ILL", sigstr)!=0) return SIGILL ;
	if ( strstr("ABR", sigstr)!=0) return SIGABRT;
	if ( strstr("FPE", sigstr)!=0) return SIGFPE ;
	if ( strstr("KIL", sigstr)!=0) return SIGKILL;
	if ( strstr("SEG", sigstr)!=0) return SIGSEGV;
	if ( strstr("PIP", sigstr)!=0) return SIGPIPE;
	if ( strstr("ALR", sigstr)!=0) return SIGALRM;
	if ( strstr("TER", sigstr)!=0) return SIGTERM;
	if ( strstr("USR1",sigstr)!=0) return SIGUSR1;
	if ( strstr("USR2",sigstr)!=0) return SIGUSR2;
	if ( strstr("CHL" ,sigstr)!=0) return SIGCHLD;
	if ( strstr("CON" ,sigstr)!=0) return SIGCONT;
	if ( strstr("STO" ,sigstr)!=0) return SIGSTOP;
	if ( strstr("TST" ,sigstr)!=0) return SIGTSTP;
	if ( strstr("TTI" ,sigstr)!=0) return SIGTTIN;
	if ( strstr("TTO" ,sigstr)!=0) return SIGTTOU;
	fprintf(stderr,"%s: %s est un signal non reconnu\n",
			progname,define);
	return -1;
}

void signal_print()
{
	printf("signal SIGHUP = %d\n",SIGHUP  );
	printf("signal SIGINT = %d\n",SIGINT  );
	printf("signal SIGQUIT= %d\n",SIGQUIT );
	printf("signal SIGILL = %d\n",SIGILL  );
	printf("signal SIGABRT= %d\n",SIGABRT );
	printf("signal SIGFPE = %d\n",SIGFPE  );
	printf("signal SIGKILL= %d\n",SIGKILL );
	printf("signal SIGSEGV= %d\n",SIGSEGV );
	printf("signal SIGPIPE= %d\n",SIGPIPE );
	printf("signal SIGALRM= %d\n",SIGALRM );
	printf("signal SIGTERM= %d\n",SIGTERM );
	printf("signal SIGUSR1= %d\n",SIGUSR1 );
	printf("signal SIGUSR2= %d\n",SIGUSR2 );
	printf("signal SIGCHLD= %d\n",SIGCHLD );
	printf("signal SIGCONT= %d\n",SIGCONT );
	printf("signal SIGSTOP= %d\n",SIGSTOP );
	printf("signal SIGTSTP= %d\n",SIGTSTP );
	printf("signal SIGTTIN= %d\n",SIGTTIN );
	printf("signal SIGTTOU= %d\n",SIGTTOU );
}

int         isnumber(const char* p)
{
	for ( ; *p ; p+=1 )
		if ( *p<'0' || '9'<*p )
			return 0;
	return 1;
}
