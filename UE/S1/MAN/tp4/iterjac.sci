exec("/home/romain.pereira/MAN/tp4/iter.sci")
exec("/home/romain.pereira/MAN/tp4/jac.sci")

//  fonction: 'GS'
//  parametre:  A : matrice
//              b : vecteur
//              n : entier
//  retour:     xn :    le vecteur xn issue de la n-ième
//                      itération appliqué au probleme A.x = b
//                      avec la méthode de Jacobi
function [xn] = iterjac(A, b, n)
    [J, bt] = jac(A, b)
    x0 = bt  // ???
    xn = iter(J, bt, x0, n)
endfunction
