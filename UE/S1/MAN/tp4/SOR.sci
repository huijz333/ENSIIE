exec("/home/romain.pereira/MAN/tp4/decomp.sci")

//  fonction: 'SOR'
//  parametre:  A : matrice
//              b : vecteur
//  retour:     (Lw, bt) : matrice de SOR et vecteur bt associé
function [Lw, bt] = SOR(A, b, w)
    [D, E, F] = decomp(A)
    G = inv(D - w * E)
    Lw = G * (w * F + (1 - w) * D)
    bt = w * G * b
endfunction
