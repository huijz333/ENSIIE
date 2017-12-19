//  fonction: 'trigGauss'
//  parametre:  A : matrice trigonalisable
//              b : vecteur réel
//  retour:     (At, bt) : vecteur solution de A.x = b <=> At.x = bt , avec At triangulaire superieur

function [At,bt] = trigGauss(A, b)
    n = size(A, 1)
    eps = 0.0000000000000001
    for k = 1:n-1
        if abs(A(k, k)) < eps
            disp("un message d erreur")
        else
            for i = k+1:n
                c = A(i, k) / A(k, k)
                b(i) = b(i) - c * b(k)
                A(i, k) = 0
                for j = k+1:n
                    A(i, j) = A(i, j) - c * A(k, j)
                end
            end
        end
    end
    At = A
    bt = b
endfunction
