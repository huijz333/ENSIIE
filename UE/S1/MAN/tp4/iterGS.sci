exec("/home/romain.pereira/MAN/tp4/iter.sci")
exec("/home/romain.pereira/MAN/tp4/GS.sci")

//  fonction: 'GS'
//  parametre:  A : matrice
//              b : vecteur
//              n : entier
//  retour:     xn :    le vecteur xn issue de la n-ième
//                      itération appliqué au probleme A.x = b
//                      avec la méthode de Gauss-Seidel
function [xn] = iterGS(A, b, n)
    [L, bt] = GS(A, b)
    x0 = bt  // ???
    xn = iter(L, bt, x0, n)
endfunction
