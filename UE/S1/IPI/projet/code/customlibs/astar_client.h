#ifndef ASTAR_CLIENT_H
# define ASTAR_CLIENT_H

# include "lab.h"

/** les signaux a utiliser */
# define SIG_PRINT	SIGUSR1

/** enfant qui cherche de l'entrée à la sortie directement */
enum e_client {
	CLIENT_E_S,	/** enfant qui cherche la sortie sans passer par la porte */
	CLIENT_E_C,	/** enfant qui cherche de l'entrée à la clef */
	CLIENT_C_P,	/** enfant qui cherche de la clef à la porte */
	CLIENT_P_S,	/** enfant qui cherche de la porte à la sortie */
	MAX_CLIENTS	/** nombre maximum d'enfant */
};

/** represente un enfant qui cherche un chemin */
typedef struct	s_client {
	BYTE	id;	/** voir CLIENT_S_T, CLIENT_ [...] */
	pid_t	pid;	/** processus ID */
	WEIGHT	time;	/** le temps calculé par le processus */
}		t_client;


/** les packets disponibles dans la communication (enfant => père) */
enum e_packet {
	PACKET_ID_ENDED,
	PACKET_ID_PATHTIME
};
/** represente un paquet de donnée à écrire/lire dans les pipes
    lors de la parallélisation */
typedef struct	s_packet {
	BYTE	id;		/** ID du packet */
	BYTE	clientID;	/** id de l'enfant qui écrit */
	WEIGHT	time;		/** temps du chemin, ou INF_WEIGHT si fini sans trouvé */
}		t_packet;

/**
 *	@require : 	'lab':	le labyrinthe
 *			'sPos':	la position de départ
 *			'tPos':	la position d'arrivé
 *			'p':	le pipe dans lequel écrire les packets
 *			'clientID': l'id de l'enfant (voir CLIENT_* (astar_client.h))
 *	@ensure  : crée un nouveau processus qui cherche un chemin entre 's' et 't',
 *                 dans le labyrinthe, et qui écrit la longueur du chemin
 *		   dans le pipe 'p',
 *		   renvoie le pid du processus fils créé.
 *		   le processus se suicide à la fin de la recherche
 *	@assign  : --------------------------------------------
 */
t_client astar_client(t_lab * lab, t_pos sPos, t_pos tPos, int p[2], BYTE clientID);

#endif
