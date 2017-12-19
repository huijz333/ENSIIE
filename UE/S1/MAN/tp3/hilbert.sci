//  fonction: 'hilbert'
//  parametre:  n : ordre de la matrice voulu
//  retour:     H : la matrice d'hilbert d'ordre n
function [H] = hilbert(n)
    H = zeros(n, n)
    for i = 1:n
        for j = 1:i
            v = 1 / (i + j - 1)
            H(i, j) = v
            H(j, i) = v
        end
    end
endfunction
