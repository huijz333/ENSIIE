exec("/home/romain.pereira/MAN/tp4/decomp.sci")

//  fonction: 'GS'
//  parametre:  A : matrice
//              b : vecteur
//  retour:     (L, bt) : matrice de gauss-seidel et vecteur bt associé
function [L, bt] = GS(A, b)
    [D, E, F] = decomp(A)
    G = inv(D - E)
    L = G * F
    bt = G * b
endfunction
