exec("/home/romain.pereira/MAN/tp4/iter.sci")
exec("/home/romain.pereira/MAN/tp4/SOR.sci")

//  fonction: 'SOR'
//  parametre:  A : matrice
//              b : vecteur
//              n : entier
//              w : argument de la méthode SOR
//  retour:     xn :    le vecteur xn issue de la n-ième
//                      itération appliqué au probleme A.x = b
//                      avec la méthode de Gauss-Seidel
function [xn] = iterSOR(A, b, n, w)
    [Lw, bt] = SOR(A, b, w)
    x0 = bt  // ???
    xn = iter(Lw, bt, x0, n)
endfunction
