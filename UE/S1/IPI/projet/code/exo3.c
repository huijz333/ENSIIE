# include "customlibs/exo3.h"

void lab_solve(t_lab * lab, unsigned int timer) {
	(void)lab;
	(void)timer;
	/**

Algorithme:

	- On cherche le plus court chemin qui n'a besoin d'aucunes clefs
		=> on crée une bitmap et on résout à l'aide de l'exo 1
		=> s'il existe et s'il est suffisement court, fin

	- Sinon, on
	*/
}

int main(void) {
	t_lab * lab = lab_parse(stdin);
	if (lab == NULL) {
		return (EXIT_FAILURE);
	}

	unsigned int timer;
	scanf("%u\n", &timer);

	lab_solve(lab, timer);
	
	lab_delete(lab);

	return (0);
}
