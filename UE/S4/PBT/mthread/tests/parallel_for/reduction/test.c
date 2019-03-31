/**
 *
 *	Effectue la somme des entiers de 0 à 19 par 4 threads
 *
 * ----------------------------------------------------------
 *
 * équivalent en code OMP:
 *
 * ----------------------------------------------------------
 *
 *	int main(void) {
 * 		int i;
 * 		omp_set_num_threads(4);
 * 		int sum;
 *
 * 		# pragma omp parallel for schedule(static) reduction(+:sum)
 * 		for (i = 0 ; i < 20 ; i++) {
 * 			sum += i;
 * 		}
 * 		return 0;
 *	}
 * ----------------------------------------------------------
 */

#include <stdio.h>
#include <mthread.h>

/**
 * paramètre utilisateur, effectuant 3 sommes
 */
typedef struct {
	int S1;
	int S2;
}	t_arg;

static void run(mthread_pf_context_t * ctx) {
	t_arg * arg = (t_arg *)ctx->arg;
	MTHREAD_PF_REDUCTION(ctx, 0, arg->S1 += ctx->iterator);
	MTHREAD_PF_REDUCTION(ctx, 1, arg->S2 -= ctx->iterator);
}

int main(void) {

	/** l'entier dans lequelle on effectue le reduction */
	t_arg arg = {0, 0};

	/** création de la configuration du parallel for */
	mthread_pf_t conf;

	/* recupere la conf par défuat */
	mthread_pf_default_conf(&conf);

	/* modifie les paramètres */
	conf.num_threads	= 4;
	conf.schedule		= MTHREAD_PARALLEL_FOR_STATIC;
	conf.bgn			= 0;
	conf.end			= 200;
	conf.arg			= &arg;
	conf.n_reductions	= 2; /* on effectue 2 reductions */

	/* appel */
	mthread_pf(&conf, run);

	/* affichage du résultat */
	printf("S1 = 0 + 1 + 2 + ... + %d = %d\n", conf.end, arg.S1);
	printf("S2 = 0 - 1 - 2 + ... - %d = %d\n", conf.end, arg.S2);

    return 0;
}

