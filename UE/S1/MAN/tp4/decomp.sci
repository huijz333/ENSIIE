//  fonction: 'iter'
//  parametre:  A : matrice
//  retour:     D : la diagonal de A
//              E : la partie triangulaire inférieur stricte de A
//              F : la partie triangulaire inférieur stricte de A
function [D, E, F] = decomp(A)
    n = size(A, 1)
    D = diag(diag(A))
    E = -triu(A - D)
    F = -tril(A - D)
endfunction
