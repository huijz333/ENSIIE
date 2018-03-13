/*=========================================================================
 * $cours  : TP réseau
 * $section: protocole hello
 *      $Id: hello-clt.c 451 2018-03-04 14:11:48Z ia $ 
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/reseau/programmation/src/hello-clt.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
 **=======================================================================*/

/*=======================================================================*/
/*= Implémentation d'un client "hello" en TCP/IPv4                      =*/
/*= Le protocole "hello" est décrit dans le header hello.h              =*/
/*=                                                                     =*/
/*= Usage:                                                              =*/
/*=    $ ./a.out serveur port                                           =*/
/*=                                                                     =*/
/*= Paramètre:                                                          =*/
/*=    serveur: le nom du serveur (www.ensiie.fr, localhost) ou son IP  =*/
/*=             en "doted format" (127.0.0.1, 172.30.253.2)             =*/
/*=    port: le port du serveur qui est un entier positif.              =*/
/*=======================================================================*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "hello.h"

static void gest_sigpip(int sig) {
	fprintf(stderr, "\n%s serveur deconnecté\n", prgname);
	exit(1);
}

int main(int argc, char** argv)
{
	prgname = argv[0];
	int statut;

	signal(SIGPIPE, gest_sigpip);

	/* vérification des arguments */
	int tp = argc == 4 ? atoi(argv[3]) * 100000 : 0;
	if ( argc!=3 && argc != 4 || tp < 0) {
		fprintf(stderr,"%s:usage %s serveur port {tp}\n",prgname,prgname);
		exit(1);
	}
	char* namesvr = argv[1];
	char* service = argv[2];

	/* création du SAP du serveur */
	socklen_t       svrSAPlen;
	struct sockaddr svrSAP;
	getTCPsap(&svrSAP,&svrSAPlen,namesvr,service);

	/* connexion au serveur */
	int cx;
	if ( (cx=socket(AF_INET, SOCK_STREAM,0))==-1 ) {
		fprintf(stderr,"%s: pb socket: %s\n",argv[0],strerror(errno));
		exit(1);
	}
	if ( connect(cx,&svrSAP,svrSAPlen)==-1 ) {
		fprintf(stderr,"%s: pb connect: %s\n",argv[0],strerror(errno));
		exit(1);
	}

	/** pause */
	pause();
	
	return 1;
}
