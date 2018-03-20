/*=========================================================================
 * $cours  : TP réseau
 * $section: protocole hello
 *      $Id: hello-svr.c 451 2018-03-04 14:11:48Z ia $ 
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/reseau/programmation/src/hello-svr.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
 **=======================================================================*/

/*=======================================================================*/
/*= Implémentation d'un serveur "hello" en TCP/IPv4                     =*/
/*= Le protocole "hello" est décrit dans le header hello.h              =*/
/*=                                                                     =*/
/*= Usage:                                                              =*/
/*=    $ ./a.out port                                                   =*/
/*=                                                                     =*/
/*= Paramètre:                                                          =*/
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

void gest_sigpipe(int err) {
	printf("\n%s : client deconnecté.\n", prgname);
}

int main(int argc, char** argv)
{
	prgname = argv[0];
	int statut;

	/* vérification des arguments */
	int tp = argc == 3 ? atoi(argv[2]) * 100000 : 0;
	if ( argc!=2 && argc != 3 || tp < 0) {
		fprintf(stderr,"%s:usage: %s port {tp}\n",prgname,prgname);
		exit(1);
	}
	char* service = argv[1];

	/* création du SAP des clients */
	socklen_t       cltsSAPlen;
	struct sockaddr cltsSAP;
	getTCPsap(&cltsSAP,&cltsSAPlen,NULL, service);

	/* création de l'automate de connexion */
	int sock;
	if ( (sock=socket(AF_INET,SOCK_STREAM,0))==-1 ) {
		fprintf(stderr,"%s: pb socket: %s\n",argv[0],strerror(errno));
		exit(1);
	}
	if ( bind(sock,&cltsSAP,cltsSAPlen)<0 ) {
		fprintf(stderr,"%s: pb bind: %s\n",argv[0],strerror(errno));
		exit(1);
	}
	if ( listen(sock,100)!=0 ) {
		fprintf(stderr,"%s: pb listen:%s\n",argv[0],strerror(errno));
		exit(1);
	}

	signal(SIGPIPE, gest_sigpipe);

	while (1) {
		int cx;
		struct sockaddr cltSAP;
		socklen_t       cltSAPlen=sizeof(cltSAPlen);

		/* creation du flux de communication (cx) */
		if ( (cx=accept(sock,&cltSAP,&cltSAPlen))==-1 ) {
			fprintf(stderr,"%s: pb accept : sock=%d : %s\n",argv[0],sock,strerror(errno));
			exit(1);
		}

		char PDU[100];
		/* Dialogue état début */
		statut = write(cx,"HELLO",6);
		if ( statut==-1 ) { close(cx); continue; }
		printf("serveur: envoyé \"HELLO\" : "); fflush(stdout);
		/* Dialogue état att-ok */
		statut = lire_PDU(PDU,cx);
		if ( statut!='O' ) goto error;
		printf("reçu \"%s\" :", PDU); fflush(stdout);
		if (tp != 0) {
			usleep(tp);
		}
		statut = write(cx,"FIN",4);
		if ( statut==-1 ) { close(cx); continue; }
		printf(" envoyé \"FIN\" : quitte\n");

		/* Terminaison du dialogue (état fin) */
		close(cx);
		continue;

error:
		close(cx);
		fprintf(stderr,"%s: message de type %c est inattendu\n",
				prgname,statut);
	}

	close(sock);
	return 0;
}
/*=======================================================================*/
