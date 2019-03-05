#include <stdio.h>
#include <mthread.h>

int main(void) {
    mthread_t thrd;
    mthread_create(&thrd, NULL, (void * (*)(void *))puts, "Hello world");
    mthread_join(thrd, NULL);
    puts("Success");
    return 0;
}
