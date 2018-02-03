# exo1: #
#### Q-1 : ####
``` echo "un" > 1 ```
#### Q-2 : ####
``` echo "un" > /pub/1 ```
#### Q-3 : ####
``` ls /pub > /pub/file ```
#### Q-4 : ####
``` echo $PATH > /pub/path ```
#### Q-5 : ####
``` find /pub -name "*.c" -or -name "*.h" ```
ou plus court (mais moins générique)
``` find /pub -name "*.[ch] ```


# exo4: #

### Q-1 : ###
Le fichier "out" contiendra 2 fois les octets du fichier "in", les octets sont écris dans l'ordre,
mais peuvent "se chevaucher".

exemple:
• si "in" contient "123", alors "out" pourra contenir "123123", ou encore "112323", ou encore "112233"

Ceci est du au fait que 2 processus écrivent en meme temps dans le même pipe, on ne peut à priori pas savoir lequel des deux va écrire en premier.
Cependant, chaque processus écrit l'integralité du fichier 1 fois, dans l'ordre

### Q-2 ###
ce programme ne s'arrête pas car le "read()" dans le processus pere est bloquant et ne se terminera jamais.

Le pipe 'com[]' est ouvert en écriture et lecture dans le père et les 2 fils.
Une fois les 2 fils terminés, il y a donc encore un écrivain potentiel dans le pipe,
et donc le 'read()' est bloquant.

### Q-3 ###
Pour que le programme s'arrête, il suffit de rajouter ligne 58,
``` C
close(com[1]);
```
car le père n'a à priori pas besoin d'écrire dans le pipe, et le programme se termine donc quand les 2 processus fils finissent.

# exo5: #
Programme "comment-ca-va" (version courte sans gérer les cas d'erreurs)

``` C
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main(void) {
    /** status à envoyer dans le 'wait' */
    int exitStatus;
    pid_t pid1 = fork();
    /** si on est dans le fils */
    if (pid1 == 0) {
        pid_t pid2 = fork();
        /** si on est dans le petit-fils */
        if (pid2 == 0) {
            printf("comment ");
            exit(EXIT_SUCCESS);
        }
        while (wait(&exitStatus));
        printf(" ca ");
        exit(EXIT_SUCESS);
    }
    /** on attends que le fils ait fini */
    int exitStatus;
    while (wait(&exitStatus));
    printf(" va\n);
    return (0);
}

```
Programme "comment-ca-va" (version longue en gérant les cas d'erreurs)

``` C
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main(void) {
    /** status à envoyer dans le 'wait' */
    int exitStatus;
    pid_t pid1 = fork();
    /** si on est dans le fils */
    if (pid == -1) {
        fprintf(stderr, "child fork() failed\n");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0) {
        pid_t pid2 = fork();
        if (pid == -1) {
            exit(EXIT_FAILURE);
        }
        
        /** si on est dans le petit-fils */
        if (pid2 == 0) {
            printf("comment ");
            exit(EXIT_SUCCESS);
        }
        while (wait(&exitStatus));
        printf(" ca ");
        exit(EXIT_SUCESS);
    }
    /** on attends que le fils ait fini */
    int exitStatus;
    while (wait(&exitStatus));
    if (exitStatus == EXIT_FAILURE) {
        fprintf(stderr, "grand-child fork() failed\n");
        return (EXIT_FAILURE);
    }
    printf(" va\n);
    return (EXIT_SUCCESS);
}

```
