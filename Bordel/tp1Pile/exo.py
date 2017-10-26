""" Fonction renvoyant une pile vide """
def creer_pile():
    pile = []
    return pile

""" Fonction renvoyant le résultat dun test en booléen (true ou false) """
def est_vide(p):
    return len(p) == 0

""" Fonction empilant l'élément x dans la pile p """
def empiler(p, x):
    p.append(x)

""" fonction dépilant la pile p et renvoyant l'élément dépilé """
def depiler(p):
    return p.pop()

""" 
Fonction renvoyant le sommet (dernier terme) de la pile p sans enlever ce terme de p si la pile 
est non vide, sinon retourne None
"""
def sommet(p):
    if est_vide(p):  # si la pile est vide
        return None  # on renvoit None
    x = depiler(p)   # sinon, on depile le sommet x
    empiler(p, x)    # on rempile le sommet x
    return x         # on renvoit x

""" Fonction qui échange le sommet et le terme juste en dessous dans la pile p """
def echange_sommet(p):
    # la pile est :  [*, ..., *, y, x]
    x = depiler(p) # [*, ..., *, y] 
    y = depiler(p) # [*, ..., *]
    empiler(p, x)  # [*, ..., *, x]
    empiler(p, y)  # [*, ..., *, x, y] ... done

"""
Fonction renvoyant la taille de la pile p sans pour autant que p soit vide à la fin
(on pourra passer par une seconde pile)
"""
def taille(p):
    copy = creer_pile() #pile temporaire on which we stack elements popped from 'p'
    length = 0

    #on depile 'p' et on enregistre les valeurs dans 'copy'
    while not est_vide(p):
        empiler(copy, depiler(p))
        length = length + 1

    # now, 'length' vaux la longueur de la pile 'p', et 'p' est vide
    # on rempile 'copy' dans 'p'
    while not est_vide(copy):
        empiler(p, depiler(copy))
    return length

"""
Fonction qui duplique une pile p sans utiliser légalité entre 2 piles qui lie deux noms au mme objet
on a donc à la sortie la pile initiale(non modifiée en mémoire) et une nouvelle pile qui sera renvoyée par la fonction
"""
def duppliquer(p):
    copy = creer_pile() # pile temporaire
    q = creer_pile()    # duplicat de 'p'

    # algorithm: on depile 'p' dans 'copy'
    while not est_vide(p):
        empiler(copy, depiler(p))

    # puis on depile 'copy' dans 'p' et 'q'
    while not est_vide(copy):
        x = depiler(copy)
        empiler(p, x)
        empiler(q, x)

    # on renvoie le duplicat de 'p'
    return q

""" fonction vidant la pile 'p' """
def vider(p):
    while not est_vide(p):
        depiler(p)

""" renvoie la pile inverse de p """
def inverser(p):
    q = creer_pile() # pile inverse de p
    while not est_vide(p):
        empiler(q, depiler(p))
    return q

"""
Fonction qui ajoute les éléments dune pile q à la pile p.
Une fois exécutée, la pile q doit etre inchangée. Cela revient à poser q sur p
"""
def empiler_piles(p, q):
    iq = inverser(q)
    afficher(iq)
    while not est_vide(iq):
        empiler(p, depiler(iq))

"""
Fonction transforme la pile p en plaçant son sommet tout en bas
et tous les autres termes étant remontés d'un cran
"""
def cycler(p):
    # p = [a, b, ..., z, x]
    x = depiler(p)
    # p = [a, b, ..., z]
    ip = inverser(p)
    # ip = [z, ..., b, a]

    vider(p)
    empiler(p, x)
    # p = [x]
    while not est_vide(ip):
        empiler(p, depiler(ip))
    # p = [x, a, b, ..., z] : done

# TEST A PARTIR D'ICI
# TEST A PARTIR D'ICI
# TEST A PARTIR D'ICI
# TEST A PARTIR D'ICI
# TEST A PARTIR D'ICI
# TEST A PARTIR D'ICI
# TEST A PARTIR D'ICI
# TEST A PARTIR D'ICI


""" affiche le contenu de la pile """
def afficher(p):
    # sur le meme model que 'taille(p)'
    copy = creer_pile()
    s = '' # chaine de caractere qui contiendra la pile sous forme de character
    while not est_vide(p):
        x = depiler(p)
        empiler(copy, x)
        s = str(x) + " -> " + s
    print(s[0:-4:])
    
    while not est_vide(copy):
        empiler(p, depiler(copy))


# debut
p = creer_pile()
print("------------------------------------")
print("creation de la pile...")
for ch in "Yasmine":
    empiler(p, ch)
afficher(p)


print("------------------------------------")
print("calcul de la taille de la pile...")
t = taille(p)
afficher(p)
print("taille de la pile: " + str(t))


print("------------------------------------")
print("échange des sommets...")
echange_sommet(p)
afficher(p)

print("------------------------------------")
print("inversion de la pile...")
afficher(inverser(p))


print("------------------------------------")
print("empiler_pile...")
p = creer_pile()
q = creer_pile()
for c in "Yas":
    empiler(p, c)
for c in "mine":
    empiler(q, c)
afficher(p)
afficher(q)
empiler_piles(p, q)
afficher(p)


print("------------------------------------")
print("cycler...")
cycler(p)
afficher(p)
