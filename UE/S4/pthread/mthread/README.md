# TD1
**2.1** Bibliothèque thread au niveau utilisateur uniquement

Architecture:
	- 1 processus système avec 1 thread système
	- ordonnanceur en espace utilisateur
	- N thread virtuel en espace utilisateur, pouvant prendre la main sur le thread système

Avantages:
	- Simple à implémenter
	- Performante, on limite le nombre de processus sur le système (et donc le nombre de changement de contexte)
	- Entièrement en espace utilisateur => simple à maintenir
	
Inconvénients:
	- Pas adaptée au SMP et multicoeur, car un seul processus noyau
	- Problèmes avec les appels systèmes.

**2.2** Bibliothèque thread au niveau système uniquement

Architecture:
	- 1 processus système avec N threads
	- ordonnancement en espace système seulement
	- chaque thread de l'espace utilisateur sont directement liés à 1 thread système.

Avantages:
	- Adaptée au SMP et multicoeur (car developpé dans le système)
	- Gère bien les appels systèmes (car developpé dans le système)

Inconvénients
	- Complexe à mettre en oeuvre (demande une modification au niveau système)
	- Coût plus élevé (augmente le travail de l'ordonnanceur système)

**2.3** Bibliothèque thread mixte (utilisateur + noyau)

Architecture:
	- 1 processus système avec N threads système
	- M threads virtuel utilisateur, avec N <= M
	- ordonnancement en espace utilisateur et système
	- l'ordonnanceur en espace utilisateur choisit les threads virtuels utilisateurs devant prendre la main sur un thread système.


**3.1**
| Bibliothèque | Performance | Flexibilité | SMP/NUMA | Appels systèmes bloquants |
|--------------|-------------|-------------|----------|---------------------------|
| Utilisateur  | +           | +           | -        | -                         |
| Système      | -           | -           | +        | +                         |
| Mixte        | +           | +           | ++       | Limité                    |

# TD2

**2.1** : mthread.c

**2.2**
L'ordonnanceur utilisateur possède 'MTHREAD\_MAX\_VIRTUAL\_PROCESSORS' (= 64) processeurs virtuels (donc coté utilisateur)

Lors de la création du 1er thread, la bibliothèque est initialisé, à partir de la fonction '\_\_mthread\_lib\_init'.
Les différents structures sont alloués en mémoire, et les processeurs virtuels sont initialisés dans un état (attribut *state*) est mis à 0.


Lorsqu'un thread est crée, il est initialisé puis est ajouter à la liste des threads prêt à l'execution du processeur virtuel 0. (*vp->read\_list*)


'mthread\_yield':


**2.3**
Ligne **23** : 'mthread\_list\_init'
Ligne **34** : 'mthread\_insert\_first'
Ligne **47** : 'mthread\_insert\_last'
Ligne **61** : 'mthread\_remove\_first'

**2.4**

