//  fonction: 'Cholesky'
//  parametre:  A : matrices triangulaire symetrique definie positive
//  retour:     B : matrice B triangulaire inférieur tel que A = tB.B

function [B] = Cholesky(A)
    B = A
    n = size(A, 1)
    for j = 1:n
        for i = j:n
            B(i, j) = B(i, j) - B(i, 1:j-1) * B(j, 1:j-1)'
            if i == j then,
                B(i, j) = sqrt(B(i, j))
            else
                B(i, j) = B(i, j) / B(j, j)
            end
        end
    end
    
    for j = 1:n
        for i = 1:j-1
            B(i, j) = 0
        end
    end
endfunction

//function [B] = Cholesky(A)
//    n = size(A, 1)
//    [B, U] = FactLU(A)
//    for i = 1:n
//        Uii = sqrt(U(i,i))
//        for j = i:n
//            B(j,i) = B(j,i)*Uii
//        end
//    end
//endfunction
