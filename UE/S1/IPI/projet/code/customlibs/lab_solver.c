# include "lab.h"

/**
 *	@require : le labyrinthe, un sommet de départ 's', et un
 *		   sommet d'arrivée 't'.
 *	@ensure  : effectue l'algorithme de remontée de 's' à 't',
 *		   et affiche les directions successives à prendre
 *		   pour realiser le chemin.
 *	@assign  : ------------------
 */
void lab_print_path(t_lab * lab, t_node * s, t_node * t) {

	/** algorithme de remontée */
	t_list * path = list_new();
	while (t != s) {
		list_push(path, t, sizeof(t_node));
		t = t->prev;
	}
	list_push(path, s, sizeof(t_node));

	/** sinon, on prends les 2 sommets de chaque arc du chemin */
	t_list_node * first = path->head->next;
	t_list_node * second = path->head->next->next;
	while (second != path->head) {
		/** la coordonnée des 2 sommets */
		t_node * u = (t_node *)(first + 1);
		t_node * v = (t_node *)(second + 1);
		/** si les 2 cases sont identiques, et sont un teleporteur */
		if (lab->map[u->pos.y][u->pos.x] == lab->map[v->pos.y][v->pos.x]
				&& lab_get_tpID(lab->map[u->pos.y][u->pos.x]) != MAX_TP) {
			printf("TP\n");
		} else {
		/** sinon on cherche la direction, et on l'affiche */
			BYTE i;
			int dx = (int)v->pos.x - (int)u->pos.x;
			int dy = (int)v->pos.y - (int)u->pos.y;
			for (i = 0 ; i < MAX_DIRECTIONS ; i++) {
				t_direction d = DIRECTIONS[i];
				if (dx == d.x && dy == d.y) {
					printf("%s\n", d.name);
					break ;
				}
			}
		}
		/** passe au 2 sommets suivant */
		first = first->next;
		second = second->next;
	}

	/* libere la mémoire */
	list_delete(path);
}

/**
 *	@require : le labyrinthe, et le timeout
 *	@ensure  : tente de résoudre le labyrinthe dans le temps imparti.
 *		   	1) s'il n'y a pas de portes, on résout avec A*.
 *			2) sinon, on résout en parallèle les 4 problemes de
 *			chemin suivant, avec A*:
 *				1.1) chemin ne passant pas par la porte
 *				1.2) chemin allant du départ à la clé
 *				1.3) chemin allant de la clé à la porte
 *				1.4) chemin allant de la porte à la clef
 *			La parallélisation s'arrête si 1) convient,
 *			ou si la concaténation de 2), 3) et 4) convient.
 *
 *			Si un chemin convient, le signal 'SIG_PRINT' est
 *			envoyé à l'enfant, qui affiche son chemin.
 *
 *		Renvoie 1 si un chemin a été trouvé et affiché,
 *		sinon, 0 est renvoyé et "Not connected" est affiché
 *	@assign  : -----------------------------------------
 */
int lab_solve(t_lab * lab, WEIGHT maxTime) {

	/** s'il n'y a pas de portes */
	if (lab->door.x == MAX_NODES) {
		/** on résout directement, pas de parallélisation*/
		return (astar(lab, lab->entry, lab->exit, maxTime));
	}



	/** sinon, on essaye de résoudre en parallèle sans passer par la porte,
	    et en passant par la porte */
	int p[2];
	pipe(p);

	/** creation de 1.1, 1.2, 1.3, 1.4 */
	t_worker workers[MAX_WORKERS];
	workers[WORKER_E_S] = astar_worker(lab, lab->entry, lab->exit, p, WORKER_E_S);
	workers[WORKER_E_C] = astar_worker(lab, lab->entry, lab->key,  p, WORKER_E_C);
	workers[WORKER_C_P] = astar_worker(lab, lab->key,   lab->door, p, WORKER_C_P);
	workers[WORKER_P_S] = astar_worker(lab, lab->door,  lab->exit, p, WORKER_P_S);
	BYTE i;
	for (i = 0 ; i < MAX_WORKERS ;  i++) {
		if (workers[i].pid == -1) {
			fprintf(stderr, "A fork failed.\n");
			return (0);
		}
	}

	/** sinon, on est dans le père */ 
	/** le père n'écrit pas, il ne fait que lire */
	close(p[1]);

	/* on lit les réponses des fils */
	int status; /** status des processsus apres avoir été kill */
	BYTE ended = 0; /** nombre de processus en attente d'affichage ou de terminaison */
	t_packet packet;
	while (read(p[0], &packet, sizeof(t_packet)) > 0) {
		BYTE wID = packet.workerID;
		switch (packet.id) {
			/** si c'est un paquet de mis à jour */
			case PACKET_ID_PATHTIME:
				/** on enregistre le temps pour cet enfant */
				workers[wID].time = packet.time;

				/** on lit le paquet */
				switch (wID) {
					/** si c'est le cas 1.1 */
					case WORKER_E_S:
						/** si le chemin est assez court */
						if (workers[WORKER_E_S].time <= maxTime) {
							/** on l'affiche */
							kill(workers[WORKER_E_S].pid, SIG_PRINT);
							/** et on tue les enfants */
							for (i = 0 ; i < MAX_WORKERS ; i++) {
								if (i != WORKER_E_S) {
									kill(workers[i].pid, SIGKILL);
								}
							}
							/** on attends que tous les enfants aient fini */
							int status;
							while (wait(&status) > 0);
							return (1);
						}
						break ;
						/** sinon, si c'est 1.2, 1.3 ou 1.4 */
					default:
						/** si la concatenation des
						  chemins convient */
						if (	workers[WORKER_E_C].time != INF_WEIGHT &&
								workers[WORKER_C_P].time != INF_WEIGHT &&
								workers[WORKER_P_S].time != INF_WEIGHT &&

								workers[WORKER_E_C].time
								+ workers[WORKER_C_P].time
								+ workers[WORKER_P_S].time <= maxTime) {
							/** on tue le processus qui cherche un chemin
							  sans passer par la porte */
							kill(workers[WORKER_E_S].pid, SIGKILL);

							/** on affiche le chemin (entrée, clef) */
							kill(workers[WORKER_E_C].pid, SIG_PRINT);
							waitpid(workers[WORKER_E_C].pid, &status, 0);

							/** on affiche le chemin (clef, porte) */
							kill(workers[WORKER_C_P].pid, SIG_PRINT);
							waitpid(workers[WORKER_C_P].pid, &status, 0);

							/** on affiche le chemin (porte, sortie) */
							kill(workers[WORKER_P_S].pid, SIG_PRINT);
							waitpid(workers[WORKER_P_S].pid, &status, 0);
							/** succès */
							return (1);
						}
						break ;
				}
				break ;

			/** si c'est un paquet de terminaison d'un enfant*/
			case PACKET_ID_ENDED:

				/** si ce processus a terminé,
				    c'est qu'il n'y a pas de chemins assez
				    court sans passer par la porte, on le tue */
				if (wID == WORKER_E_S) {
					/** on tue le processus */
					kill(workers[WORKER_E_S].pid, SIGKILL);
				}
				/** si l'un des chemins:
				    de l'entrée à la clef,
				    de la clef à la porte,
				    ou de la porte à la sortie,
				    n'existe pas,
				 
				    ou si tous les programmes ont finis,
				    que leur concatenation reste trop longue...
				 */
				if ((workers[wID].time == INF_WEIGHT && wID != WORKER_E_S)
						|| ++ended == MAX_WORKERS) {
					/** alors, ca ne sert à rien que
					    les autres continuent de chercher,
					    il n'y a pas de solutions */
					kill(workers[WORKER_E_C].pid, SIGKILL);
					kill(workers[WORKER_C_P].pid, SIGKILL);
					kill(workers[WORKER_P_S].pid, SIGKILL);
					fprintf(stderr, "Key, door, or exit couldn't be reached.\n");
				}
				break ;

			/** sinon, c'est un packet inconnu.. PROBLEME */
			default:
				fprintf(stderr, "Unknown packet (%u) received from %u\n",
						packet.id, wID);
				break ;
		}
	}

	/** si read renvoit 0 ou -1,
	  	<=> tous les pipes ont été fermé 
	  	<=> tous les enfants sont morts
		<=> pas de chemin trouvé */
	fprintf(stderr, "Pas de chemins valides!\n");
	return (0);
}

