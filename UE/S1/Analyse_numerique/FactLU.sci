//  fonction: 'FactLU' , calcul L et U tel que A = L.U
//  parametre:  A : matrice
//  retour:     L : L matrice triangulaire inferieur
//              U : U matrice triangulaire superieur
function [L, U] = FactLU(A)
    n = size(A,1)
    L = eye(n, n)
    U = zeros(n,n)
    for i = 1:n-1
        for j = i:n
            U(i,j) = A(i,j) - L(i,1:i-1) * U(1:i-1,j)
        end
        for j =i+1:n
            L(j,i) = 1 / U(i,i) * (A(j,i) - L(j, 1:i-1) * U(1:i-1, i))
        end
    end
    U(n,n) = A(n,n) - L(n, 1:n-1) * U(1:n-1,n)
endfunction
