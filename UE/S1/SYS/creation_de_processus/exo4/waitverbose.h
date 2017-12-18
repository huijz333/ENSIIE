#ifndef FILE_WAITVERBOSE_H
#define FILE_WAITVERBOSE_H

#include <syscall.h>
#include <errno.h>

// Comme wait mais imprime sur stderr un message clair.
// valeur renvoyée:
//   -1: pas de processus fils.
//   >0: le processus # s'est terminé.
int waitendverbose(int*status)
{
    int ret,etat;
    int thid=syscall(SYS_gettid); // le pid dans tous les cas
    ret = wait(&etat);
    if ( ret<0 ) {
        fprintf(stderr,"%d: no event to wait for: %s\n",
                thid,strerror(errno));
        return -1;
    }
    if ( WIFEXITED(etat) ) {
        fprintf(stderr,"%d: child %d exited with status %d\n",
                thid,ret, WEXITSTATUS(etat));
        if (status) *status=etat;
        return ret;
    }
    if ( WIFSIGNALED(etat) ) {
        fprintf(stderr,"%d: child %d exited due to signal %d\n",
                thid,ret, WTERMSIG(etat));
        if (status) *status=etat;
        return ret;
    }
    fprintf(stderr,"%d: child %d terminated for a ? reason\n",
            thid,ret);
    if (status) *status=etat;
    return ret;
}

// Attend les événements fin, suspension et réveil d'un fils,
// Renvoie le pid du fils dans *fils et imprime sur stderr
// un message clair.
// valeur renvoyée:
//   -1: pas de processus fils.
//    0: le processus fils s'est terminé.
//    1: le processus fils est stoppé.
//    2: le processus fils est réactivé.
int waitallverbose(int*fils)
{
    int ret,status;
    int thid=syscall(SYS_gettid); // le pid dans tous les cas
    ret = waitpid(-1,&status,WUNTRACED|WCONTINUED);
    if ( ret<0 ) {
        fprintf(stderr,"%d: no event to wait for: %s\n",
                thid,strerror(errno));
        return -1;
    }
    if ( WIFEXITED(status) ) {
        fprintf(stderr,"%d: child %d exited with status %d\n",
                thid,ret, WEXITSTATUS(status));
        if (fils) *fils=ret;
        return 0;
    }
    if ( WIFSIGNALED(status) ) {
        fprintf(stderr,"%d: child %d exited due to signal %d\n",
                thid,ret, WTERMSIG(status));
        if (fils) *fils=ret;
        return 0;
    }
    if ( WIFSTOPPED(status) ) {
        fprintf(stderr,"%d: child %d stoped due to signal %d\n",
                thid,ret, WSTOPSIG(status));
        if (fils) *fils=ret;
        return 1;
    }
    if ( WIFCONTINUED(status) ) {
        fprintf(stderr,"%d: child %d resumed by signal %d\n",
                thid,ret, SIGCONT);
        if (fils) *fils=ret;
        return 2;
    }
    fprintf(stderr,"%d: child %d terminated for a ? reason\n",
            thid,ret);
    if (fils) *fils=ret;
    return 0;
}

#endif


