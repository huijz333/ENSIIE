//  fonction: 'solinf'
//  parametre:  A : matrices triangulaire inferieur
//              b : vecteur réel
//  retour:     x : vecteur solution de A.x = b

function x = solinf_loop(A, b)
    n = size(A, 1)
    //check n == m
    x = zeros(n, 1)
    x(1) = b(1) / A(1, 1)
    i = 2
    while i <= n
        s = 0
        j = 1
        while j <= i - 1
            s = s + A(i, j) * x(j)
            j = j + 1
        end
        x(i) = 1 / A(i, i) * (b(i) - s)
        i = i + 1
    end
endfunction

function x = solinf(A, b)
    n = size(A, 1)
    //check n == m
    x = zeros(n, 1)
    x(1) = b(1) / A(1, 1)
    i = 2
    while i <= n
        s = A(i ; 1:i-1) * x(1:i-1)
        x(i) = 1 / A(i, i) * (b(i) - s)
        i = i + 1
    end
endfunction
