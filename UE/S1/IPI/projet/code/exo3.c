# include "customlibs/exo3.h"

int main(void) {
	/** lit le labyrinthe sur l'entrée standart */
	t_lab * lab = lab_parse();
	if (lab == NULL) {
		fprintf(stderr, "Error on lab parsing\n");
		return (EXIT_FAILURE);
	}

	/** lit le timeout */
	WEIGHT timer;
	scanf(WEIGHT_IDENTIFIER "\n", &timer);

	/** resolution du graphe */
	lab_solve(lab, timer);
	/** libere la mémoire */
	lab_delete(lab);

	return (0);
}
