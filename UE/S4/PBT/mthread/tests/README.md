Chaque test évalue une fonctionnalité de la bibliothèque.
Le test est expliqué en haut du fichier correspondant.

Pour compiler tous les tests
> make

Pour compiler et lancer tous les tests
> make test

Pour compiler/recompiler un seul test:
> make [nom]

exemple:
> make thread

Puis lancer les tests un par un:
> ./thread/thread
> ./mutex/init
[...]
