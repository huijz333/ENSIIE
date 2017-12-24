# include "lab.h"

/**
 *	NB : ce constructeur est relativement long, car on s'assure
 *		qu'il n'y ait pas de fuite mémoire en cas d'erreur
 *
 *	@require :	n : longueur (== largeur) du labyrinthe
 *	@ensure	 :	crée un nouveau labyrinthe de taille 'n'
 *	@assign  :	-------
 */
t_lab * lab_new(INDEX n) {
	t_lab * lab = (t_lab *) malloc(sizeof(t_lab));
	if (lab == NULL) {
		return (NULL);
	}
	lab->n = n;
	lab->teleporters = hmap_new(16, (t_hashf)inthash, (t_cmpf)intcmp);
	if (lab->teleporters == NULL) {
		free(lab);
		return (NULL);
	}
	lab->doors = hmap_new('z' - 'a' + 1, (t_hashf)inthash, (t_cmpf)intcmp);
	if (lab->doors == NULL) {
		hmap_delete(lab->doors);
		free(lab);
		return (NULL);
	}
	lab->map = (char **) malloc(sizeof(char *) * n);
	if (lab->map == NULL) {
		hmap_delete(lab->doors);
		hmap_delete(lab->teleporters);
		free(lab);
		return (NULL);
	}

	INDEX i;
	for (i = 0 ; i < lab->n ; i++) {
		lab->map[i] = (char *) malloc(sizeof(char) * (lab->n + 1));
		if (lab->map[i] == NULL) {
			INDEX j;
			for (j = 0 ; j < i ; j++) {
				hmap_delete(lab->teleporters);
				hmap_delete(lab->doors);
				free(lab->map[j]);
				free(lab->map);
				free(lab);
			}
			return (NULL);
		}
	}
	return (lab);
}

/**
 *	@require :	lab : un labyrinthe alloué via 'lab_new()'
 *	@ensure	 :	supprime le labyrinthe du tas
 *	@assign  :	-------
 */
void lab_delete(t_lab * lab) {
	INDEX i;
	for (i = 0 ; i < lab->n ; i++) {
		free(lab->map[i]);
	}
	free(lab->map);
	hmap_delete(lab->teleporters); /*TODO : free keys and values */
	hmap_delete(lab->doors);
	free(lab);
}

/**
 *	@require :	un flux
 *	@ensure	 :	alloue et lit un nouveau labyrinthe sur le flux donnée
 *	@assign  :	-------
 */
t_lab * lab_parse(FILE * stream) {
	INDEX n;
	fscanf(stream, INDEX_IDENTIFIER "\n", &n);
	t_lab * lab = lab_new(n);
	if (lab == NULL) {
		return (NULL);
	}

	INDEX i;
	for (i = 0 ; i < n ; i++) {
		fscanf(stream, "%s\n", lab->map[i]);
	}
	return (lab);
}

