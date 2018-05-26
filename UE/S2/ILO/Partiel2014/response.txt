sujet: https://www.iiens.net/etudiants/cours/S2/ILO-Langages_objet/Partiels/2013-2014/ILO%20-%20Partiel%20-%20Sujet.pdf

1) a. (voir cours)

2) d. ('package-private', voir https://stackoverflow.com/questions/16164902/what-is-the-default-access-modifier-in-java)

3) c. (https://www.tutorialspoint.com/cplusplus/cpp_class_access_modifiers.htm)

4) c. (https://stackoverflow.com/questions/860339/difference-baetween-private-public-and-protected-inheritance)

5) c. (par élimination...)

6) a. pour les objets, b. pour les types primitifs

7) c. le pointeur 'this' ne peut pas être nulle (il est définit lors de l'allocation/construction de l'objet,
si cette étape échoue, alors l'objet n'existe pas, 'this' non plus...)
Par exemple, si un 'new' échoue en Java, une 'NotEnoughMemoryException' est levé.

8) a. et c. (https://fr.wikipedia.org/wiki/Surcharge_(programmation_informatique))

9) a. et d.

10) b. et d.

11) a. et b.

12) b. (https://pastebin.com/H7YwQhVE) , c. est discutable, car les attributs sont initialisés à 0 (ou null), mais ceci meme si un autre constructeur est appelé.
ou alors le constructeur par défaut est toujours appelé? bref

13) b.

14) c. : un constructeur ne retourne rien, il construit un objet dont on a une référence non nulle

15) b. et c. (http://en.cppreference.com/w/cpp/language/copy_constructor)

16) a. (https://stackoverflow.com/questions/347358/inheriting-constructors)

17) b. et d.

18) b. et c.

19) d. (par élimination... bien que ce soit une mauvaise pratique, c'est cependant possible que d. soit faux:
https://pastebin.com/X5JQKktT

20) étrange comme question, ca sert à templater son code pour éviter d'avoir à recodder 20 fois la même chose
b. à priori? (List<String>, List<Integer>, List<Object>...)

21) lol really : a. (sinon 'error: generic array creation' )

22) ...
https://docs.oracle.com/javase/7/docs/api/java/util/LinkedList.html
b. d. et e.

23) cf 21) et 22) : a. b. c.

24) b.

25) a. (voir http://uml.free.fr/cours/p15.html)

26) b.

27)

28)

29) a. et d.

-------------

1) "A() "

2) "A() "
   "B() "

3a) "A.f0() "

4a) pete une erreur, f1 not found (tableau de 'A', pas de 'B')

3a) "B.f0() "

4a) pete une erreur, f1 not found (tableau de 'A', pas de 'B')


--------
3.a)
```
for (E e : this.elements) {
  System.out.println(e);
}
```

3.b)
```
for (Iterator<E> it = this.elements.iterator() ; it.hasNext() ; ) {
  System.out.println(it.next());
}
```


3.c)
```
for (int i = 0 ; i < this.elements.size() ; i++) {
  System.out.println(this.elements.elementAt(i));
}
```
