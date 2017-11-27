#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

// renvoie 1 si str est un nombre en base 10
// renvoie 0 dans le cas contraire 
int isNumber(const char* str)
{
  for ( ; *str ; str+=1) 
    if ( *str<'0' || '9'<*str )
      return 0;
  return 1;
}

char tmp[300];
char* envv_next[4] = {tmp, tmp+100, tmp+200, 0};
char* argv_next[1] = {0};

int main(int argc, char*argv[], char* envv[])
{
  if ( argc==1 ) {
    // première exécution
    if ( !isNumber( basename(argv[0])) ) {
      fprintf(stderr,"%s:usage: number\n", argv[0]);
      exit(1);
    }
    envv_next[0] = basename(argv[0]);
    envv_next[1] = ... ;
    envv_next[2] = ... ;
  } else if ( argc==0 ) {
    // exécutions suivantes
    int n     = ... ;
    int fac   = ... ;
    char* cmd = ... ;
    if ( n==1 ) {
      printf("factoriel de %s = %d\n",basename(cmd),fac);
      return 0;
    }
    fac = ...; n = ...;
    sprintf(envv_next[0],...);
    sprintf(envv_next[1],...);
    envv_next[2] = ...;
  } else {
      fprintf(stderr,"%s:usage: %s\n", argv[0],argv[0]);
      exit(1);
  }
  execve(...);
  fprintf(stderr,"pb execve(%s...) : %s\n",
      envv_next[2],strerror(errno));
  return 1;
}

