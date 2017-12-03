## GENERALITES ##

# EXO 1 #
- Les arcs d'un graphe sont representés sous la structure 't_bitmap'
	- cette structure contient un entier 'n', le nombre de sommet du graphe
	- les coefficients de la matrice sont alloués directement apres la structure:
		[...] - [ int ] - [ coefficients ] - [ ... ]
	Etant donnée que les coefficients valent '0' ou '1', ils sont codés sur un 'bit'.
	La zone mémoire 'coefficients' est un tableau d'octet.
	On crée la bijection:

		F : (i, j) -> i * n + j
	
	Le coefficient (i, j) de la matrice est mappé sur le F(i, j)ème bit du tableau


