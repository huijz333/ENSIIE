//  fonction: 'iterSORtest'
//  parametre:  A : matrice
//              b : vecteur
//              x0 : vecteur
//              n : entier
//              eps : petit espilon, test d'arrêt
//              w : argument de la méthode de SOR
//  retour:     xn :    le vecteur xn issue de la n-ième, ou le 1er
//                      vecteur qui passe le teste d'arrêt, lors de
//                      l'itération appliqué au probleme A.x = b
//                      avec la méthode de Gauss-Seidel, ave
//              i : le nombre d'itération effectué
function [xn, i] = iterSORtest(A, b, x0, n, eps, w)
    [Lw, bt] = SOR(A, b, w)
    
    xn = x0
    for i = 1:n
        if (norm(A * xn -  b) / norm(b) <= eps) then
            return
        end
        xn = Lw * xn + bt
    end
endfunction
