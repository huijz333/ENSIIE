//  fonction: 'solinf'
//  parametre:  A : matrices triangulaire inferieur
//              b : vecteur réel
//  retour:     x : vecteur solution de A.x = b

function x = solinf(A, b)
    n = size(A, 1)
    x = zeros(n, 1)
    x(1) = b(1) / A(1, 1)
    for i = 2:n
        x(i) = 1 / A(i, i) * (b(i) - A(i,1:i-1) * x(1:i-1))
    end
endfunction
