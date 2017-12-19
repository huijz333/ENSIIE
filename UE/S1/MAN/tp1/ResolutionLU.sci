exec("/home/romain.pereira/MAN/tp1/FactLU.sci")
exec("/home/romain.pereira/MAN/tp1/solinf.sci")
exec("/home/romain.pereira/MAN/tp1/solsup.sci")

//  fonction: 'ResolutionLU'
//  parametre:  A : matrice 
//              b : vecteur réel
//  retour:     x : vecteur solution de A.x = b
function [x] = ResolutionLU(A, b)
    [L, U] = FactLU(A)  // A = L . U
    y = solinf(L, b)    //  A.x = b <=> L . (U.x) = b <=> L.y = b, avec y = U.x
    x = solsup(U, y)    // U.x = y <=> L.U.x = b <=> A.x = b
endfunction
