/*=========================================================================
 * $cours  : TP réseau
 * $section: protocole qrac
 *      $Id: qrac-clt.c 451 2018-03-04 14:11:48Z ia $ 
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/reseau/programmation/src/qrac-clt.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
**=======================================================================*/

/*=======================================================================*/
/*= Implémentation d'un client "QRAC" en TCP/IPv4                       =*/
/*= Le protocole "QRAC" est décrit dans le header qrac.h                =*/
/*=                                                                     =*/
/*= Usage:                                                              =*/
/*=    $ ./a.out serveur port nq                                        =*/
/*=                                                                     =*/
/*= Paramètre:                                                          =*/
/*=    serveur: le nom du serveur (www.ensiie.fr, localhost) ou son IP  =*/
/*=             en "doted format" (127.0.0.1, 172.30.253.2)             =*/
/*=    port: le port du serveur qui est un entier positif.              =*/
/*=    nq:   le numéro de la question (un entier positif ou nul).       =*/
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

#include "qrac.h"

void gest_sigpipe(int sig)
{
    fprintf(stderr,"%s: serveur déconnecté\n",prgname);
    exit(1);
}

int main(int argc, char** argv)
{
  prgname = argv[0];
  int statut;

  /* vérification des arguments */
  if ( argc!=4 ) {
      fprintf(stderr,"%s:usage %s serveur port nq\n",prgname,prgname);
      exit(1);
  }
  char* namesvr   = argv[1];
  char* service   = argv[2];
  int   nquestion = atoi(argv[3]);
  if ( nquestion<0 ) {
      fprintf(stderr,"%s: %d est un numéro de question invalide\n",prgname,nquestion);
      exit(1);
  }

  signal( SIGPIPE, gest_sigpipe );

  /* création du SAP du serveur */
  socklen_t       svrSAPlen;
  struct sockaddr svrSAP;
#warning TODO

  /* connexion au serveur */
  int cx;
#warning TODO

  char pdu[100];
  /* dialogue: etat REPOS */
  statut = write(cx,"H",2);

  /* dialogue: etat ATTC */
  statut = lire_PDU(pdu,cx);
  if ( statut!='C' ) goto fin_dialogue_inattendue;
  unsigned short v = extrait_N_de_PDUcrq(pdu) ^ cle;
  gen_PDUcrq(pdu,'R', v+1);
  statut = write(cx,pdu,3);

  /* dialogue: etat CHALLENGE */
  statut = lire_PDU(pdu,cx);
#warning TODO

  /* dialogue: etat ACCEPT */
  statut = lire_PDU(pdu,cx);
#warning TODO

  /* dialogue: etat REPOS (FIN) */
  close(cx);
  return 0;

fin_dialogue_inattendue:
  fprintf(stderr,"%s: message de type '%c' est inattendu\n",
          prgname,statut);
fin_dialogue_erreur:
  close(cx);
  return 1;
}
