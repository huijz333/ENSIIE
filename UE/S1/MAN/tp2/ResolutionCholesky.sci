exec("/home/romain.pereira/MAN/tp2/Cholesky.sci")
exec("/home/romain.pereira/MAN/tp1/solinf.sci")
exec("/home/romain.pereira/MAN/tp1/solsup.sci")

//  fonction: 'ResolutionCholesky'
//  parametre:  A : matrices triangulaire symetrique definie positive
//              b : vecteur colonne
//  retour:     x : vecteur colonne tel que A.x = b
function [x] = ResolutionCholesky(A, b)
    B = Cholesky(A)
    y = solinf(B, b)
    x = solsup(B', y)
endfunction
