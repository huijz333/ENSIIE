#ifndef ASTAR_WORKER_H
# define ASTAR_WORKER_H

# include "lab.h"

/** les signaux a utiliser */
# define SIG_PRINT	SIGUSR1

/** enfant qui cherche de l'entrée à la sortie directement */
enum e_worker {
	WORKER_E_S,	/** enfant qui cherche la sortie sans passer par la porte */
	WORKER_E_C,	/** enfant qui cherche de l'entrée à la clef */
	WORKER_C_P,	/** enfant qui cherche de la clef à la porte */
	WORKER_P_S,	/** enfant qui cherche de la porte à la sortie */
	MAX_WORKERS	/** nombre maximum d'enfant */
};

/** represente un enfant qui cherche un chemin */
typedef struct	s_worker {
	BYTE	id;	/** voir WORKER_S_T, WORKER_ [...] */
	pid_t	pid;	/** processus ID */
	WEIGHT	time;	/** le temps calculé par le processus */
}		t_worker;


/** les packets disponibles dans la communication (enfant => père) */
enum e_packet {
	PACKET_ID_ENDED,
	PACKET_ID_PATHTIME
};
/** represente un paquet de donnée à écrire/lire dans les pipes
    lors de la parallélisation */
typedef struct	s_packet {
	BYTE	id;		/** ID du packet */
	BYTE	workerID;	/** id de l'enfant qui écrit */
	WEIGHT	time;		/** temps du chemin, ou INF_WEIGHT si fini sans trouvé */
}		t_packet;

/**
 *	@require : 	'lab':	le labyrinthe
 *			'sPos':	la position de départ
 *			'tPos':	la position d'arrivé
 *			'p':	le pipe dans lequel écrire les packets
 *			'workerID': l'id de l'enfant (voir WORKER_* (astar_worker.h))
 *	@ensure  : crée un nouveau processus qui cherche un chemin entre 's' et 't',
 *                 dans le labyrinthe, et qui écrit la longueur du chemin
 *		   dans le pipe 'p',
 *		   renvoie le pid du processus fils créé.
 *		   le processus se suicide à la fin de la recherche
 *	@assign  : --------------------------------------------
 */
t_worker astar_worker(t_lab * lab, t_pos sPos, t_pos tPos, int p[2], BYTE workerID);

#endif
