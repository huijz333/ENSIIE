#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

void init_mo(int nb)
{
    void* p;
    int i,j;
    for (i=0; i<nb ; i++) {
        for (j=0 ; j<(1<<20) ; j+=(1<<12)) {
            p = malloc((1<<12)-16);
        }
    }
}

int hello(void *p) {
    execlp("echo","echo","hello ",NULL);
    exit(1);
}

int main(int argc,char*argv[])
{
    if ( argc!=2 ) {
      fprintf(stderr,"%s:usage %s n-mo\n",
          argv[0],argv[0]);
      exit(1);
    }

    int nbmo = atoi(argv[1]);
    int status,i;

    init_mo(nbmo);

    for (i=0 ; i<1000 ; i++) {
        if ( fork()==0 ) hello(NULL);
        wait(&status);
    }

    return 0;
}
