# include "lab.h"

/**
 *	@require : le labyrinthe et une liste correspondant au chemin
 *	@ensure  : affiche les directions successives à prendre pour
 *		   realiser le chemin.
 *	@assign  : ------------------
 */
static void lab_print_path(t_lab * lab, t_list * path) {
	/** si le chemin est nulle... */
	if (path == NULL) {
		printf("Not connected\n");
		return ;
	}

	/** sinon, on prends les 2 sommets de chaque arc du chemin */
	t_list_node * first = path->head->next;
	t_list_node * second = path->head->next->next;
	while (second != path->head) {
		/** la coordonnée des 2 sommets */
		t_pos * u = (t_pos *)(first + 1);
		t_pos * v = (t_pos *)(second + 1);
		/** si les 2 cases sont identiques, et sont un teleporteur */
		if (lab->map[u->y][u->x] == lab->map[v->y][v->x]
				&& lab_get_tpID(lab->map[u->y][u->x]) != MAX_TP) {
			printf("TP\n");
		} else {
		/** sinon on cherche la direction, et on l'affiche */
			BYTE i;
			int dx = (int)v->x - (int)u->x;
			int dy = (int)v->y - (int)u->y;
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
 *			ou si la concaténation de 2), 3) et 4) convient
 *	@assign  : -----------------------------------------
 */
void lab_solve(t_lab * lab, size_t timer) {

	/** s'il n'y a pas de portes */
	if (lab->door.x == MAX_NODES) {
		/** on résout directement, pas de parallélisation*/
		t_list * path = astar(lab, lab->entry, lab->exit, timer);
		lab_print_path(lab, path);
		list_delete(path);
		return ;
	}


	/** sinon, on essaye de résoudre en parallèle sans passer par la porte,
	    et en passant par la porte */
	int p[2];
	pipe(p);

	/** creation de 1.1 */
	pid_t pid1 = fork();
	if (pid1 == 0) {
		/** si on est dans le fils */
		lab_delete(lab);
		exit(EXIT_SUCCESS);
	}

	/** sinon, on est dans le père */ 
	/** le père n'écrit pas, il ne fait que lire */
	close(p[1]);

	/* on lit les résultats des fils */
	t_packet packet;
	while (read(p[0], &packet, sizeof(t_packet)) > 0) {
		printf("packet read: %hhu %lu\n", packet.childID, packet.timer);
	}
}


