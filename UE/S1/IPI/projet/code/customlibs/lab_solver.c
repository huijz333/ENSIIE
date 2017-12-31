# include "lab.h"

/**
 *	@require : le labyrinthe, et un chemin
 *	@ensure  : affiche sur la sortie standart les directions
 *			successives à prendre pour réaliser le chemin
 *	@assign  : -------------------------
 */
static void lab_print_path(t_array * nodes, t_array * path) {
	/** recupere le 1er sommet */
	t_nodel * prev = array_get(nodes, *((INDEX *) array_get(path, 0)));
	INDEX i;
	for (i = 1 ; i < path->size ; i++) {
		/** on recupere le 2eme sommet */
		INDEX nextID = *((INDEX *) array_get(path, i));
		t_nodel * next = (t_nodel *) array_get(nodes, nextID);
		/** si les 2 sommets sont des téléporteurs */
		if (prev->id == next->id && prev->id != LAB_EMPTY) {
			printf("TP\n");
		} else {
		/** sinon, on recupere la direction du déplacement */
			int dx = (int)next->x - (int)prev->x;
			int dy = (int)next->y - (int)prev->y;
			BYTE j;
			for (j = 0 ; j < MAX_DIRECTIONS ; j++) {
				t_direction d = DIRECTIONS[j];
				if (dx == d.x && dy == d.y) {
					/** on l'affiche */
					printf("%s\n", d.name);
					break ;
				}
			}
		}
		/** on passe à l'arc suivant */
		prev = next;
	}
}

/** deux fonctions heuristiques d'exemple */
WEIGHT heuristic_diagonal(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)uID;
	t_nodel * v = (t_nodel *) array_get(nodes, vID);
	(void)sID;
	(void)tID;
	return (v->x - v->y);
}

WEIGHT heuristic_manhattan(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)uID;
	t_nodel * v = (t_nodel *) array_get(nodes, vID);
	(void)sID;
	t_nodel * t = (t_nodel *) array_get(nodes, tID);
	return (t->x - v->x + t->y - v->y);
}

/**
 *	@require : le graphe (tableau de sommets), l'indice de départ,
 *		   l'indice d'arrivé, et un pointeur sur un timer.
 *	@ensure  : tente de trouver un chemin entre 's' et 't' avec un poids < timer.
 *		   Renvoie 1 si un chemin a été trouvé, 0 sinon
 *	@assign  : les sommets des graphes sont modifiés par 'dijkstra()'
 *		   si un chemin est trouvé, le 'timer' est diminué par le poids du chemin.
 */
static int lab_try_solve(t_array * nodes, INDEX s, INDEX t, WEIGHT * timer) {
	/** on résout avec dijkstra */
/*	int r = astar(nodes, heuristic_diagonal, s, t);*/
/*	int r = astar(nodes, heuristic_manhattan, s, t);*/
	int r = dijkstra(nodes, s, t);
	WEIGHT pathtime = ((t_nodew *)array_get(nodes, t))->pathw;

	/** si la resolution a echoué, ou prends trop de temps */
	if (r == 0 || pathtime > *timer) {
		/** on renvoie 0 */
		return (0);
	}

	/** sinon, on construit le chemin */
	t_array * path = node_build_path(nodes, s, t);
	if (path == NULL) {
		array_delete(path);
		return (0);
	}
	
	lab_print_path(nodes, path);
	array_delete(path);

	*timer -= pathtime;

	return (1);
}


/**
 *	@require : le labyrinthe
 *	@ensure  : crée les arcs entre la porte et ses voisins
 *	@assign  : ---------
 */
static void lab_open_door(t_lab * lab) {
	t_array * nodes = lab->nodes;
	t_nodew * door = (t_nodew *) array_get(nodes, lab->door);
	BYTE i;
	for (i = 0 ; i < MAX_DIRECTIONS ; i++) {
		/** si le voisin est dans le graphe */
		INDEX vID = lab->vdoor[i];
		if (vID != MAX_NODES) {
			/** on crée les arcs avec ce voisin */
			t_nodew * v = (t_nodew *) array_get(nodes, vID);
			if (v->ws == NULL) {
				v->ws = array_new(1, sizeof(WEIGHT));
				v->super.successors = array_new(1, sizeof(INDEX));
			}
			WEIGHT w = 1;
			array_add(v->ws, &w);
			array_add(v->super.successors, &(lab->door));

			/** et reciproquement*/
			if (door->ws == NULL) {
				door->ws = array_new(1, sizeof(WEIGHT));
				door->super.successors = array_new(1, sizeof(INDEX));
			}
			array_add(door->ws, &w);
			array_add(door->super.successors, &vID);
		}
	}
}

/**
 *	@require : un labyrinthe valide (renvoyé via 'lab_parse()'), et un temps 'timer'
 *	@ensure  : tente de resoudre le labyrinthe en moins de 'timer' opérations.
 *			Renvoie 1 en cas de succes, 0 sinon
 *	@assign  : les sommets du graphe representant le labyrinthe peuvent
 *			être modifié par l'algorithme a*
 */
int lab_solve(t_lab * lab, WEIGHT timer) {
	/** les sommets du graphe du labyrinthe */
	t_array * nodes = lab->nodes;
	INDEX n = lab->nodes->size;
	if (lab->entry >= n || lab->exit >= n) {
		fprintf(stderr, "Entry or exit nodes are invalid\n");
		return (0);
	}
	/** on essaye de le resoudre en considerant les portes comme des murs */
	if (lab_try_solve(nodes, lab->entry, lab->exit, &timer)) {
		/** si succès fin */
		return (1);
	}

	/** sinon, s'il y a une clef et une porte */
	if (lab->key != MAX_NODES && lab->door != MAX_NODES) {
		/* on essaye de resoudre en allant chercher la clef d'abord */
		if (lab_try_solve(nodes, lab->entry, lab->key, &timer)) {
			/** si on a reussit a recuperer la clef */
			/** on crée les arcs entre la porte et ses voisins */
			lab_open_door(lab);

			/** on essaye d'aller à la sortie à partir de la clef */
			if (lab_try_solve(nodes, lab->key, lab->exit, &timer)) {
				/** si succès, on renvoie 1 */
				return (1);
			}
		}
	}
	
	/** sinon, s'il n'y a pas de portes, ou si meme en allant chercher
	    les clefs, la resolution a echoué c'est que la sortie est
	    innaccessible avec avec le 'timer' donné */
	printf("Not connected\n");
	return (0);
}
