/**
 *
 * Parallélisation d'une simple boucle 'for'
 * qui affiche 16 entiers à partir de 4 threads
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
 *
 * 		# pragma omp parallel for schedule(static)
 * 		for (i = 0 ; i < 16 ; i++) {
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
	conf.num_threads = 4;
	conf.schedule = MTHREAD_PARALLEL_FOR_STATIC;
	conf.bgn = 0;
	conf.end = 16;

	/** appel */
	mthread_parallel_for(&conf, run);

    return 0;
}

