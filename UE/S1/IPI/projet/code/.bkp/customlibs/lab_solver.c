# include "lab.h"

/** definition des téléporteurs */
char LAB_TP[MAX_TP] = "*%$#&+-@^!£";

/** définition des directions possibles de deplacement. */
t_direction DIRECTIONS[MAX_DIRECTIONS] = {
	{"DROITE",	 1,  0},
	{"GAUCHE",	-1,  0},
	{"HAUT",	 0, -1},
	{"BAS",		 0,  1}
/*	{"BAS/GAUCHE",	-1,  1},
	{"BAS/DROITE",	 1,  1},
	{"HAUT/GAUCHE",	-1, -1},
	{"HAUT/DROITE",	 1, -1}*/
};

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

/** 3 fonctions heuristiques d'exemple */

/** fonction heuristique orientant la recherche sur la diagonale */
WEIGHT heuristic_diagonal(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)uID;
	t_nodel * v = (t_nodel *) array_get(nodes, vID);
	(void)sID;
	(void)tID;
	return (v->x - v->y);
}

/** fonction heuristique orienté la recherche en fonction de la proximité du sommet 't' */
WEIGHT heuristic_manhattan(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)uID;
	t_nodel * v = (t_nodel *) array_get(nodes, vID);
	(void)sID;
	t_nodel * t = (t_nodel *) array_get(nodes, tID);
	return (t->x - v->x + t->y - v->y);
}

/** fonction heuristique nulle (<=> dijkstra) */
WEIGHT heuristic_zero(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)nodes;
	(void)uID;
	(void)vID;
	(void)sID;
	(void)tID;
	return (0);
}

/** wrapper pour appelé 'astar' avec l'heuristique de manhattan */
static int astar_manhattan(t_array * nodes, INDEX s, INDEX t) {
	return (astar(nodes, heuristic_manhattan, s, t));
}


/**
 *	@require : le graphe (tableau de sommets), l'indice de départ,
 *		   l'indice d'arrivé, un pointeur sur un timer,
 *		   et une fonction de résolution de plus court chemin
 *	@ensure  : tente de trouver un chemin entre 's' et 't' avec un poids < timer.
 *		   Renvoie 1 si un chemin a été trouvé, 0 sinon
 *	@assign  : les sommets des graphes sont modifiés par 'dijkstra()'
 *		   si un chemin est trouvé, le 'timer' est diminué par le poids du chemin.
 */
static int solve(t_array * nodes, INDEX s, INDEX t, WEIGHT * timer,
				int (*solve)(t_array *, INDEX, INDEX)) {
	/** on résout avec dijkstra */
	int r = solve(nodes, s, t);
	WEIGHT pathtime = ((t_nodew *)array_get(nodes, t))->pathw;

	/** si la resolution a echoué, ou prends trop de temps */
	if (r == 0 || pathtime > *timer) {
		return (0);
	}

	/** sinon, on diminue le temps */
	*timer -= pathtime;

	return (1);
}


/**
 *	@require : le labyrinthe
 *	@ensure  : crée les arcs entre la porte et ses voisins
 *	@assign  : ---------
 */
static int lab_open_door(t_lab * lab) {
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
	return (1);
}

static int lab_close_door(t_lab * lab) {
	t_array * nodes = lab->nodes;
	t_nodew * door = (t_nodew *) array_get(nodes, lab->door);
	array_delete(door->ws);
	array_delete(door->super.successors);
	door->ws = NULL;
	door->super.successors = NULL;
	return (1);
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
	INDEX s = lab->entry;	/** entrée = start */
	INDEX t = lab->exit;	/** sortie = target */
	INDEX k = lab->key;	/** clef   = key */
	INDEX d = lab->door;	/** porte  = door */

	if (lab->entry >= n || lab->exit >= n) {
		fprintf(stderr, "Entry or exit nodes are invalid\n");
		return (0);
	}

	/** 1. on essaye de le resoudre en considerant les portes comme des murs */
	/** en utilisant A* avecl'heuristique distance de manhatthan,
	    si la solution trouvé n'est pas suffisement courte, on résout
	    avec dijkstra */
	if (solve(nodes, s, t, &timer, astar_manhattan)
			|| solve(nodes, s, t, &timer, dijkstra)) {
		/** si succès fin */
		t_array * s_to_t = node_build_path(nodes, s, t);
		lab_print_path(nodes, s_to_t);
		array_delete(s_to_t);
		return (1);
	}
	
	/** 2. sinon, s'il y a une clef et une porte */
	if (k != MAX_NODES && d != MAX_NODES) {
		/* 2.1 on résout en allant cherche la clef d'abord,
		   en croisant les algorithmes de dijkstra et A* */
		
		/** on enregistre le timer pour le remettre à 0 si une resolution échoue */
		WEIGHT resetTimer = timer;

		/** clef avec A* , puis porte avec A* ou Dijksra */
		if (solve(nodes, s, k, &timer, astar_manhattan)) {
			t_array * s_to_k = node_build_path(nodes, s, k);
			lab_open_door(lab);
			if (solve(nodes, k, t, &timer, astar_manhattan)
					|| solve(nodes, k, t, &timer, dijkstra)) {

				lab_print_path(nodes, s_to_k);
				array_delete(s_to_k);

				t_array * k_to_t = node_build_path(nodes, k, t);
				lab_print_path(nodes, k_to_t);
				array_delete(k_to_t);
				return (1);
			}
			lab_close_door(lab);
			timer = resetTimer;
		}

		/** clef avec Dijkstra, puis porte A* ou Dijkstra */
		if (solve(nodes, s, k, &timer, dijkstra)) {
			t_array * s_to_k = node_build_path(nodes, s, k);
			lab_open_door(lab);
			if (solve(nodes, k, t, &timer, astar_manhattan)
					|| solve(nodes, k, t, &timer, dijkstra)) {

				lab_print_path(nodes, s_to_k);
				array_delete(s_to_k);

				t_array * k_to_t = node_build_path(nodes, k, t);
				lab_print_path(nodes, k_to_t);
				array_delete(k_to_t);
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
