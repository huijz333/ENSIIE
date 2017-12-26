# include "customlibs/exo3.h"

int main(void) {
	/** lit le labyrinthe sur l'entrée standart */
	t_lab * lab = lab_parse(stdin);
	if (lab == NULL) {
		return (EXIT_FAILURE);
	}

	/** lit le timeout */
	unsigned int timer;
	scanf("%u\n", &timer);

	/** tentative de resolution */
	lab_solve(lab, timer);
	
	/** on vide la mémoire */
	lab_delete(lab);

	return (0);
}
