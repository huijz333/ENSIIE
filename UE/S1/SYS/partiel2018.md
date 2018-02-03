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
