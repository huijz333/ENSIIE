/*=========================================================================
 * $cours  : TP réseau
 * $section: protocole hello
 *      $Id: hello.h 451 2018-03-04 14:11:48Z ia $ 
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/reseau/programmation/src/hello.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
**=======================================================================*/

/*=======================================================================*/
/*= Le protocole "hello"                                                =*/
/*=                                                                     =*/
/*= Service:  aucun                                                     =*/
/*=                                                                     =*/
/*= Automate:                                                           =*/
/*=            ?H !O           ?F                                       =*/
/*=           ------- att-fin -------       client                      =*/
/*=          /                       \                                  =*/
/*=    début                           fin                              =*/
/*=          \ !H              ?O !F /                                  =*/
/*=           ------- att-fin -------       serveur                     =*/
/*= Attention: le premier PDU est émis par le serveur.                  =*/
/*=                                                                     =*/
/*= PDU:                                                                =*/
/*=    H: 'H' 'E' 'L' 'L' 'O' '\0'         (6 octets)                   =*/
/*=    F: 'F' 'I' 'N' '\0'                 (4 octets)                   =*/
/*=    O: 'O' 'K' '\0'                     (3 octets)                   =*/
/*=======================================================================*/

/*=======================================================================*/
#ifndef FILE_HELLO_H
#define FILE_HELLO_H
/*=======================================================================*/

const char* prgname; // le nom du programme pour les message d'erreur.
int   silence = 0;   // 1: ne pas afficher les mess de debug et/ou d'information

/*=======================================================================*/

/**
 * Si host est non nul getTCPsap initialise *sap et *saplen pour un client
 * TCP/IPv4 sur le serveur host:port.
 * Si host est nul elle initialise *sap et *saplen pour un serveur TCP/IPv4
 * acceptant les connexions des clients *:port.
 *
 * host peut être donné en doted format ou en nom "humain" (host ou
 * host.doman).
 * port peut être un nom de service ou un entier.
 *
 * En cas d'échec, getTCPsap affiche un message d'erreur et termine le
 * processus avec un statut de 1. 
**/
void getTCPsap(struct sockaddr* sap, socklen_t*saplen,
              const char *host, const char* port)
{
  int status;
  struct addrinfo hints,*found;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags    = host!=0 ? 0 : AI_PASSIVE;
  hints.ai_family   = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  status=getaddrinfo(host, port,&hints,&found);
  if ( status!=0 ) {
      fprintf(stderr,"%s: pb getaddrinfo : %s\n",prgname,gai_strerror(status));
      exit(1);
  }
  *sap    = *found->ai_addr;
  *saplen =  found->ai_addrlen;
#if 0 // debug
  struct addrinfo *p;
  for ( p=found ; p!=0 ; p = p->ai_next ) {
      struct sockaddr_in *sap =  p->ai_addr;
      short port = ntohs(sap->sin_port);
      printf("prot=%d/%08x len=%d sap=%p addr=%x port=%d %d\n",p->ai_protocol,p->ai_protocol,
              p->ai_addrlen,p->ai_addr,
              sap->sin_addr.s_addr, sap->sin_port, port);

  }
#endif
  freeaddrinfo(found);
}

/*=======================================================================*/

/**
 * Lit n octets sur le flux cx et les range dans le tampon buf.
 *
 * Le tampon buf doit être alloué par l'appelant et sa taille doit
 * être d'au moins n octets.
 *
 * Retour:
 *  La fonction renvoie un nombre négatif si une erreur de lecture
 *  s'est produite.
 *  Elle renvoie n si les n octets ont été lus.
**/
int lire_data(char*buf, int cx, int n)
{
    int i,statut;
    for (i=0 ; i<n ; i++) {
      if ( (statut=read(cx,buf+i,1))!=1 ) {
        if ( ! silence )
            fprintf(stderr,"%s: pb lecture : %s\n",prgname,
                statut<0 ? strerror(errno) : "fin inattendue");
        return -1;
      }
    }
    return n;
}

/**
 * La fonction lire_PDU lit sur le flux cx un PDU du protocole HELLO et
 * le range dans le tampon buf.
 *
 * Le tampon buf doit être alloué par l'appelant et être assez grand
 * pour contenir le plus grand PDU.
 *
 * Retour
 *  Si la fonction a réussi à lire un PDU complet et correct, elle
 *  renvoie le premier octet du PDU (buf[0]).
 *  Si le pdu lu est mal formaté elle renvoie 'e'.
 *  Dans les autres cas elle renvoie '?'.
**/
int lire_PDU(char*buf, int cx)
{
    int nb;
    if ( lire_data(buf,cx,1)!=1 ) return '?';
    if ( *buf=='H' ) {
        nb =  lire_data(buf+1,cx,5); 
        return nb==5 && strcmp(buf,"HELLO")==0 ? 'H' : 'e';
    } else if ( *buf=='O' ) {
        nb =  lire_data(buf+1,cx,2); 
        return nb==2 && strcmp(buf,"OK")==0 ? 'O' : 'e';
    } else if ( *buf=='F' ) {
        nb =  lire_data(buf+1,cx,3); 
        return nb==3 && strcmp(buf,"FIN")==0 ? 'F' : 'e';
    }
    return '?';
}

/*=======================================================================*/
#endif // FILE_HELLO_H
/*=======================================================================*/

