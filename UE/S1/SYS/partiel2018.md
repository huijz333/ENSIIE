## exo4: ##

# Q-1 : #
Le fichier "out" contiendra 2 fois les octets du fichier "in", les octets sont écris dans l'ordre,
mais peuvent "se chevaucher".

exemple:
• si "in" contient "123", alors "out" pourra contenir "123123", ou encore "112323", ou encore "112233"

Ceci est du au fait que 2 processus écrivent en meme temps dans le même pipe, on ne peut à priori pas savoir lequel des deux va écrire en premier.
Cependant, chaque processus écrit l'integralité du fichier 1 fois, dans l'ordre


# Q-2 #
