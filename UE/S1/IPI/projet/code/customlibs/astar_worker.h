#ifndef ASTAR_WORKER_H
# define ASTAR_WORKER_H

# include "lab.h"

/** les signaux a utiliser */
# define SIG_SUCCESS	SIGUSR1
# define SIG_FAIL	SIGUSR2

/** un boolean indiquant si le chemin trouvé convient */
# define WORKER_STATE_RUNNING	(0)
# define WORKER_STATE_SUCCESS	(1)
# define WORKER_STATE_FAIL	(2)

/**
 *	@require : 	'lab':	le labyrinthe
 *			'sPos':	la position de départ
 *			'tPos':	la position d'arrivé
 *			'p':	le pipe dans lequel communiquer
 *			'hasKey': un boolean pour savoir si on peut passer par la porte
 *	@ensure  : crée un nouveau processus qui cherche un chemin entre 's' et 't',
 *                 dans le labyrinthe, et qui écrit la longueur du chemin
 *		   dans le pipe 'p',
 *		   renvoie le pid du processus fils créé.
 *		   le processus se suicide à la fin de la recherche
 *	@assign  : --------------------------------------------
 */
pid_t astar_worker(t_lab * lab, t_pos sPos, t_pos tPos, int p[2], BYTE hasKey);

#endif
