/**
 * 1 - Crée 2 threads
 * 2 - Les 2 threads essayent de prendre le verrou avec 'try_lock()'
 * 3 - Le thread qui parvient à prendre le verrou le garde 1 seconde
 * 4 - L'autre thread ne passer donc pas le verrou
 *
 * Résultat attendu :

[THREAD 1] tente de passer le verrou
[THREAD 1] a pris le verrou
[THREAD 2] tente de passer le verrou
[THREAD 2] n'a pas pu prendre le verrou
[THREAD 2] se termine
[THREAD 1] libere le verrou
[THREAD 1] a libéré le verrou
[THREAD 1] se termine

 */

# include <stdio.h>
# include <mthread.h>
# include <unistd.h>

mthread_mutex_t mutex;

static void * run(void * arg) {
    printf("[THREAD %ld] tente de passer le verrou\n", (long)arg);
    if (mthread_mutex_trylock(&mutex) == 0) {
        printf("[THREAD %ld] a pris le verrou\n", (long)arg);
        sleep(1);
        printf("[THREAD %ld] libere le verrou\n", (long)arg);
        mthread_mutex_unlock(&mutex);
        printf("[THREAD %ld] a libéré le verrou\n", (long)arg);
    } else {
        printf("[THREAD %ld] n'a pas pu prendre le verrou\n", (long)arg);
    }
    printf("[THREAD %ld] se termine\n", (long)arg);
    return NULL;
}

int main(int argc, char ** argv) {
    mthread_t first;
    mthread_t second;

    mthread_mutex_init(&mutex, NULL);

    mthread_create(&first, NULL, run, (void *)1);
    mthread_create(&second, NULL, run, (void *)2);

    mthread_join(first, NULL);
    mthread_join(second, NULL);

    return 0;
}
