//  fonction: 'iter'
//  parametre:  B : matrice d'itération
//              b : vecteur
//              x0: vecteur
//              n : entier
//  retour:     xn :    le vecteur xn issue de la n-ième
//                      itération de x(k + 1) = B.x(k) + b
function [xn] = iter(B, b, x0, n)
    xn = x0
    for i = 1:n
        xn = B * xn + b
    end
    return xn
endfunction
