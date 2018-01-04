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
 *		   	1) s'il n'y a pas de portes, on résout avec A*
 *			2) sinon, on résout en parallèle les 4 problemes de
 *			chemin suivant, avec A*:
 *				1.1) chemin ne passant pas par la porte
 *				1.2) chemin allant du départ à la clé
 *				1.3) chemin allant de la clé à la porte
 *				1.4) chemin allant de la porte à la clef
 *			La parallélisation s'arrête si 1) convient,
 *			ou si la concaténation de 2), 3) et 4) convient.
 *
 *			Si un chemin convient, le signal 'SIG_SUCCESS' est
 *			envoyé à l'enfant, qui affiche son chemin.
 *
 *			et alors, le signal 'SIGKILL' est envoyé aux autres.
 *
 *		Renvoie 1 si un chemin a été trouvé et affiché,
 *		sinon, 0 est renvoyé et "Not connected" est affiché
 *	@assign  : -----------------------------------------
 */
int lab_solve(t_lab * lab, WEIGHT timer) {

	/** s'il n'y a pas de portes */
	if (lab->door.x == MAX_NODES) {
		/** on résout directement, pas de parallélisation*/
		return (astar(lab, lab->entry, lab->exit, timer));
	}



	/** sinon, on essaye de résoudre en parallèle sans passer par la porte,
	    et en passant par la porte */
	int p[2];
	pipe(p);

	/** creation de 1.1 */
	pid_t pid1 = astar_worker(lab, lab->entry, lab->exit, p, 0);
	/** creation de 1.2, 1.3, 1.4 */
	pid_t pid2 = astar_worker(lab, lab->entry, lab->key,  p, 0);
	pid_t pid3 = astar_worker(lab, lab->key,   lab->door, p, 1);
	pid_t pid4 = astar_worker(lab, lab->door,  lab->exit, p, 1);

	if (pid1 == -1 || pid2 == -1 || pid3 == -1 || pid4 == -1) {
		fprintf(stderr, "Couldn't fork\n");
		return (0);
	}

	/** sinon, on est dans le père */ 
	/** le père n'écrit pas, il ne fait que lire */
	close(p[1]);

	/* on lit les résultats des fils */
	t_packet packet;
	WEIGHT s_to_k = INF_WEIGHT;
	WEIGHT k_to_d = INF_WEIGHT;
	WEIGHT d_to_t = INF_WEIGHT;
	while (read(p[0], &packet, sizeof(t_packet)) > 0) {
		if (packet.pid == pid1 && packet.timer <= timer) {
			/** si un chemin sans passer par la porte existe */
			kill(pid1, SIG_SUCCESS);
			kill(pid2, SIGKILL);
			kill(pid3, SIGKILL);
			kill(pid4, SIGKILL);
			return (1);
		} else {
			if (packet.pid == pid2) {
				/** si un chemin plus court (entrée, clef) existe */
				s_to_k = MIN(packet.timer, s_to_k);
			} else if (packet.pid == pid3) {
				/** si un chemin plus court (clef, porte) existe */
				k_to_d = MIN(packet.timer, k_to_d);
			} else if (packet.pid == pid4) {
				/** si un chemin plus court (porte, sortie) existe */
				d_to_t = MIN(packet.timer, d_to_t);
			}

			/** si la concatenation des chemins convient */
			if (	s_to_k != INF_WEIGHT &&
				k_to_d != INF_WEIGHT &&
				d_to_t != INF_WEIGHT &&
				s_to_k + k_to_d + d_to_t <= timer) {
				/** on tue le processus qui cherche un chemin
				    sans passer par la porte */
				kill(pid1, SIGKILL);
				
				/** on affiche le chemin (entrée, clef) */
				kill(pid2, SIG_SUCCESS);
				int status;
				waitpid(pid2, &status, 0);

				/** on affiche le chemin (clef, porte) */
				kill(pid3, SIG_SUCCESS);
				waitpid(pid3, &status, 0);

				/** on affiche le chemin (porte, sortie) */
				kill(pid4, SIG_SUCCESS);
				waitpid(pid4, &status, 0);

				/** succès */
				return (1);
			}


		}
	}

	/** si tous les pipes ont été fermé (<=> tous les enfants sont morts)
	    sans trouver de chemin valide... */
	printf("Not connected\n");
	return (0);
}

