/**
 * Crée un thread puis une clef, puis détruit la clef
 *
 * Résultat attendu :
 * > Success
 */

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

static void destr(void * unused) {
    puts("Key value destroyed");
}

static void * run(void * unused) {
    puts("Thread crée");
    return NULL;
}

int main(void) {
    pthread_t thrd;
    pthread_create(&thrd, NULL, run, NULL);

    pthread_key_t key;
    puts("Initializing key...");
    pthread_key_create(&key, destr);
    pthread_key_delete(key);

    puts("Done");
    puts("Joining threads");
    pthread_join(thrd, NULL);

    usleep(10000);
    puts("Success");
    return 0;
}

