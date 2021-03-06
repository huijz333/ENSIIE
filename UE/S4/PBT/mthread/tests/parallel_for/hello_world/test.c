/**
 *
 * Parallélisation d'une simple boucle 'for'
 * qui affiche 4 entiers à l'aide de 4 threads (1 entier par thread)
 *
 * ----------------------------------------------------------
 *
 * équivalent en code non parallélisé:
 *
 * ----------------------------------------------------------
 *
 *	int main(void) {
 * 		int i;
 * 		omp_set_num_threads(4);
 *
 * 		# pragma omp parallel for schedule(static)
 * 		for (i = 0 ; i < 4 ; i++) {
 * 			printf("Hello world (%d)\n", i);
 * 		}
 * 		return 0;
 *	}
 * ----------------------------------------------------------
 */

#include <stdio.h>
#include <mthread.h>

static void run(mthread_pf_context_t * ctx) {
    printf("Hello world (%d)\n", ctx->thread_id);
}

int main(void) {
	/** création de la configuration du parallel for */
	mthread_pf_t conf;

	/* recupere la conf par défuat */
	mthread_pf_default_conf(&conf);

	/* modifie les paramètres */
	conf.num_threads	= 4;
	conf.schedule		= MTHREAD_PARALLEL_FOR_STATIC;
	conf.bgn			= 0;
	conf.end			= 4;

	/** appel */
	mthread_pf(&conf, run);

    return 0;
}

