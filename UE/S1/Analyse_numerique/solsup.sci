//  fonction: 'solsup'
//  parametre:  A : matrices triangulaire superieur
//              b : vecteur réel
//  retour:     x : vecteur solution de A.x = b

function x = solsup(A, b)
    n = size(A, 1)
    x = zeros(n, 1)
    x(n) = b(n) / A(n, n)
    for i = n-1:-1:1
        x(i) = 1 / A(i, i) * (b(i) - A(i,i+1:n) * x(i+1:n))
    end
endfunction
