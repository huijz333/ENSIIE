/**
 *
 * Test que l'instanciation et la terminaison d'un thread se fait sans problème
 *
 * 1 - Crée un thread
 * 2 - Attends la fin du thread
 * 3 - Affiche 'Success'
 *
 * Résultat attendu :
 * > Success
 */

#include <stdio.h>
#include <mthread.h>

int main(void) {
    mthread_t thrd;
    mthread_create(&thrd, NULL, (void * (*)(void *))puts, "Hello world");
    mthread_join(thrd, NULL);
    puts("Success");
    return 0;
}
