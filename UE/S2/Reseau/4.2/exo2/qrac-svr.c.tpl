/*=========================================================================
 * $cours  : TP réseau
 * $section: protocole qrac
 *      $Id: qrac-svr.c 451 2018-03-04 14:11:48Z ia $ 
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/reseau/programmation/src/qrac-svr.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
**=======================================================================*/

/*=======================================================================*/
/*= Implémentation d'un serveur "QRAC" en TCP/IPv4                      =*/
/*= Le protocole "QRAC" est décrit dans le header qrac.h                =*/
/*=                                                                     =*/
/*= Usage:                                                              =*/
/*=    $ ./a.out [OPTIONS] port                                         =*/
/*=                                                                     =*/
/*= Paramètre:                                                          =*/
/*=    port: le port du serveur qui est un entier positif.              =*/
/*=                                                                     =*/
/*= Options:                                                            =*/
/*=   -h : affiche la liste des options sur stdout et quitte.           =*/
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

/*=======================================================================*/

// mange les arguments reconnus.
void parse_cmdline(int*argc, char**argv, const char*qbase);

// table des questions/réponses
// A changer pour identfier votre serveur si vous n'avez pas d'idée:
//  carré de Q, puissance de 2 de Q, Q+1, ...
static struct Tq_r { char*q; char*r; } q_r_t[] = {
    { "Gibson"   , "Mel"      }, { "Jolie"  , "Angelina" } , { "De Niro", "Robert" },
    { "Di Caprio", "Leonardo" }, { "Roberts", "Julia"    } , { "Pitt"   , "Brad"   },
    { "Willams"  , "Robin"    }, { "Ford"   , "Harrison" } , { "Cruise" , "Tom"    },
    { "Murphy"   , "Eddie"    }, { "Depp"   , "Johnny"   } , { "Willis" , "Bruce"  }
};
static int q_r_n = sizeof(q_r_t)/sizeof(q_r_t[0]);

/*=======================================================================*/

void gest_sigpipe(int sig)
{
    fprintf(stderr,"%s: client déconnecté\n",prgname);
}

int main(int argc, char** argv)
{
  prgname = argv[0];
  int statut;

  /* vérification des arguments */
  char * qbase = "Prénom de";
  parse_cmdline(&argc,argv, qbase);
  if ( argc!=2 ) {
      fprintf(stderr,"%s:usage: %s [OPTIONS] port\n",prgname,prgname);
      fprintf(stderr,"%s:usage: %s -h   (pour obtenir les options)\n",prgname,prgname);
      exit(1);
  }
  char* service = argv[1];
  printf("serveur: %d questions \"%s\" ; clé=0x%04x.\n",q_r_n,qbase, cle&0xffff);

  to = 250;  // time-out à 0.25 seconde
  signal( SIGPIPE, gest_sigpipe );

  /* création du SAP des clients */
  socklen_t       cltsSAPlen;
  struct sockaddr cltsSAP;
  getTCPsap(&cltsSAP,&cltsSAPlen,NULL,service);

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

  while (1) {
    int cx;
    struct sockaddr cltSAP;
    socklen_t       cltSAPlen=sizeof(cltSAPlen);

    /* creation du flux de communication (cx) */
    if ( (cx=accept(sock,&cltSAP,&cltSAPlen))==-1 ) {
      fprintf(stderr,"%s: pb accept : sock=%d : %s\n",argv[0],sock,strerror(errno));
      exit(1);
    }

    char pdu[100];
    /* dialogue: etat REPOS */
    statut = lire_PDU(pdu,cx);
    ...

    /* dialogue: etat ATTC */
    ...

    /* dialogue: etat ACCEPT */
    ...

    /* dialogue: etat REPOS (FIN) */
    close(cx);
    continue;

fin_dialogue_inattendue:
    fprintf(stderr,"%s: message de type '%c' est inattendu\n",
            prgname,statut);
fin_dialogue_erreur:
    close(cx);

  } // while (1)

  close(sock);
  return 0;
}

/*=======================================================================*/
/*= GLU: indispensable mais sans interêt pour un TP réseau.             =*/

void parse_cmdline(int*argc, char**argv, const char*qbase)
{
    int i,j;

    // option -v 
    for ( i=1 ; i<*argc ; i++) {
        if ( strcmp(argv[i],"-h")!=0 && strcmp(argv[i],"--help")!=0 ) continue;
        printf("usage: %s [OPTIONS] port\n",argv[0]);
        printf(
          "\n"
          "Options:\n"
          "  -h\n"
          "  --help : affiche cette aide sur stdout puis quite.\n"
          "  -l     : affiche la liste des questions puis quitte.\n"
          "  -k #   : met la clé à #, # est en hexadécimal (défaut: AA99).\n"
          "  -s     : mode silencieux.\n"
        );
        exit(0);
    }

    // option -l 
    for ( i=1 ; i<*argc ; i++) {
        if ( strcmp(argv[i],"-l")!=0 ) continue;
        printf("Listes des questions:\n");
        for ( j=0 ; j<q_r_n ;j++ ) 
            printf("  Q%3d: %s %s\n",j,qbase,q_r_t[j].q);
        exit(0);
    }

    // option -k # & -s 
    for ( i=1 ; i<*argc ; ) {
        int sh=0;
        if ( strcmp(argv[i],"-k")==0 ) {
            if (i==*argc-1) {
                fprintf(stderr,"%s: \"-k #\" option incomplète.\n",argv[0]);
                exit(1);
            }
            char* v = argv[i+1];
            if (sscanf(v,"%x",&cle)!=1) {
                fprintf(stderr,"%s: invalide valeur pour l'option \"-k %s\".\n",argv[0],v);
                exit(1);
            }
            sh=2;
        } else if ( strcmp(argv[i],"-s")==0 ) {
            silence = 1;
            sh=1;
        }
        if ( sh==0 ) {
            i+=1;
        } else {
            for ( j=i ; (j+sh)<*argc ; j++)
                argv[j] = argv[j+sh];
            *argc -= sh;
        }
    }
}

/*=======================================================================*/
