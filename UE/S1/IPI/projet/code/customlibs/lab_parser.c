# include "lab.h"

/**
 *	@require :	un flux, contenant des données supposées bien formatté
 *	@ensure	 :	alloue et lit un nouveau labyrinthe sur le flux donnée
 *	@assign  :	-------
 */
t_lab * lab_parse(void) {

	/** on alloue en mémoire le labyrinthe */
	int l;
	if (!scanf("%d\n", &l)) {
		return (NULL);
	}
	t_lab * lab = lab_new(l);
	if (lab == NULL) {
		return (NULL);
	}

	/** on lit la carte, et on l'enregistre */
	t_pos pos;
	for (pos.y = 0 ; pos.y < lab->height ; pos.y++) {
		wchar_t * line = lab->map[pos.y];
		if (!scanf("%S\n", line)) {
			fprintf(stderr, "Error reading map l:%d\n", pos.y);
			exit(EXIT_FAILURE);
		}
		for (pos.x = 0 ; pos.x < lab->width ; pos.x++) {
			BYTE tpID;
			wchar_t c = line[pos.x];
			switch(c) {
				/** si on est sur l'entrée, on définit l'entrée 's' */
				case LAB_EMPTY:
					break;
				case LAB_WALL:
					break ;
				case LAB_ENTRY:
					lab->entry = pos;
					break ;
					/** si on est sur la sortie, on définit la sortie 't' */
				case LAB_EXIT:
					lab->exit = pos;
					break ;
					/** si on est sur la clef, on l'enregistre */
				case LAB_KEY:
					lab->key = pos;
					break ;
					/** si on est sur la porte, on l'enregistre */
				case LAB_DOOR:
					lab->door = pos;
					break ;
				default:
					/** sinon */
					/** si on est sur un teleporteur */
					tpID = lab_get_tpID(c);
					if (tpID != MAX_TP) {
						/** si c'est le 1er qu'on rencontre */
						if (lab->tps[tpID][0].x == MAX_NODES) {
							lab->tps[tpID][0] = pos;
						} else {
							lab->tps[tpID][1] = pos;
						}
					} else {
						/** si on est sur une case inconnue */
						fprintf(stderr,	"unknown character : %c,"\
								"assumed '.'\n", c);
						line[pos.x] = '.';
					}
					break ;
			}
		}
	}
	return (lab);
}

