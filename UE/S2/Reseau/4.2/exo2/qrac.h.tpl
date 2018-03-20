/*=========================================================================
 * $cours  : TP réseau
 * $section: protocole qrac
 *      $Id: qrac.h 451 2018-03-04 14:11:48Z ia $ 
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/reseau/programmation/src/qrac.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
**=======================================================================*/

/*=======================================================================*/
/*= Le protocole "QRAC"                                                 =*/
/*=                                                                     =*/
/*= Services: s, rep et err.                                            =*/
/*=                                                                     =*/
/*= Automate:                                                           =*/
/*=       +-<--------------------+------------------+                   =*/
/*=       |                      | ?F !err          | ?F !err           =*/
/*=       |                      |                  |   ou              =*/
/*=       |   +---- attc ----- challenge ------+    | ?M8 !rep          =*/
/*=       |  /?s !H      ?C !R            ?O !Q \   |                   =*/
/*=     repos                                    accept                 =*/
/*=       |  \!H ?C      ?R !O                  /   |                   =*/
/*=       |   +---- atts ----------------------+    |                   =*/
/*=       |           |                             |                   =*/
/*=       |           | ?R !F                       | ?Q !F|M8          =*/
/*=       +-<---------+-----------------------------+                   =*/
/*=                                                                     =*/
/*= PDU:                                                                =*/
/*=         0   1   2  ...  9                                           =*/
/*=    H:  'H' '\0'              ( 2 octets)                            =*/
/*=    F:  'F' '\0'              ( 2 octets)                            =*/
/*=    O:  'O' '\0'              ( 2 octets)                            =*/
/*=    C:  'C'  n   n            ( 3 octets, n en big-endian, crypté)   =*/
/*=    R:  'R'  n   n            ( 3 octets, n en big-endian)           =*/
/*=    Q:  'Q'  n   n            ( 3 octets, n en big-endian)           =*/
/*=    M8: '8'  X   X  ... '\0'  (10 octets, n en big-endian)           =*/
/*=======================================================================*/

/*=======================================================================*/
#ifndef FILE_QRAC_H
#define FILE_QRAC_H

#include <poll.h>
/*=======================================================================*/

#ifndef CLE
#   define CLE ...   // valeur de la clé par défaut.
#endif

const char* prgname; // le nom du programme pour les message d'erreur.
int   silence;       // 1: ne pas afficher les mess de debug et/ou d'information
int   cle = CLE;     // la clé de cryptage/décrytage
int   to  = 0;       // timeout pour la lecture des PDU en ms (0: pas de timeout)

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
    ...
}

/*=======================================================================*/

/**
 * Lit n octets sur le flux cx et les range dans le tampon buf.
 *
 * Si la variable globale to est supérieure à zéro, la lecture est
 * abandonnée au bout de to millisecondes.
 *
 * Le tampon buf doit être alloué par l'appelant et sa taille doit
 * être d'au moins n octets.
 *
 * Retour:
 *  La fonction renvoie un nombre négatif ou nul si le timeout est
 *  atteint ou si une erreur de lecture s'est produite.
 *  Elle renvoie n si les n octets ont été lus.
**/
int lire_data(char*buf, int cx, int n)
{
    ...
}

/**
 * La fonction lire_PDU lit sur le flux cx un PDU du protocole QRAC et
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
    int statut,nb,i;
    if ( (statut=lire_data(buf,cx,1))!=1 ) {
        return '?';
    }
    switch ( *buf ) {
      case 'H': case 'O': case 'F':
        nb =  lire_data(...,...,...); 
        if ( nb<=0 )    { return 'e'; }
        if ( buf[1]!=0) { return 'e'; }
        break;
      case 'C': case 'R': case 'Q':
        ...
        ...
        break;
      case '8':
        ...
        ...
        ...
        break;
      default:
        return 'e';
    }
    return *buf;
}

/*=======================================================================*/

/**
 * Génère dans le tampon pdu:    0   1   2
 *                              type vh  vl
 * où vh:vl sont les 2 derniers chiffres en base 256 de v (big endian).
**/
void gen_PDUcrq(char* pdu, int type, unsigned int v)
{
    pdu[0] = type;
    pdu[1] = ...
    pdu[2] = ...
}

/**
 * Extrait puis renvoie la valeur n du tampon pdu contenant les données
 * d'un * PDU C, R, ou Q.
 * La valeur n est convertie dans l'endianess de la machine courante.
 * Pré-condition:
 *   pdu contient tous les octets d'un PDU C, R ou Q.
**/
int extrait_N_de_PDUcrq(char* pdu)
{
    return ...
}

/**
 * Génère dans le tampon pdu le PDU M8 contenant le message msg.
 *
 * Si msg est trop grand, il est tronqué.
 *
 * Le tampon buf doit être alloué par l'appelant et être assez grand
 * pour contenir un PDU M8.
**/
void gen_PDUm8(char* pdu, const char* msg)
{
    fprintf(stderr,"%s:%s:%d: TODO\n", prgname,__func__,__LINE__);
    exit( 1 );
}

/*=======================================================================*/
#endif // FILE_QRAC_H
/*=======================================================================*/

