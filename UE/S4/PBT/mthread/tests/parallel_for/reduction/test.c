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

typedef struct {
	mthread_mutex_t mutex;
	int sum;
}	t_arg;

static void run(mthread_pf_context_t * ctx) {
	t_arg * arg = (t_arg *) (ctx->arg);
	mthread_mutex_lock(&(arg->mutex));
	arg->sum += ctx->iterator;
	mthread_mutex_unlock(&(arg->mutex));
}

int main(void) {
	/* création de l'argument utilisateur */
	t_arg arg = {MTHREAD_MUTEX_INITIALIZER, 0};

	/** création de la configuration du parallel for */
	mthread_pf_t conf;

	/* recupere la conf par défuat */
	mthread_pf_default_conf(&conf);

	/* modifie les paramètres */
	conf.num_threads	= 4;
	conf.schedule		= MTHREAD_PARALLEL_FOR_STATIC;
	conf.bgn			= 0;
	conf.end			= 20;
	conf.arg			= &arg;

	/* appel */
	mthread_pf(&conf, run);

	/* affichage du résultat */
	printf("1 + 2 + ... + 19 = %d\n", arg.sum);

    return 0;
}

