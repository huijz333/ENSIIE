exec("/home/romain.pereira/MAN/tp1/solsup.sci")
exec("/home/romain.pereira/MAN/tp1/trigGauss.sci")

//  fonction: 'ResolutionGauss'
//  parametre:  A : matrice trigonalisable
//              b : vecteur réel
//  retour:     x : vecteur solution de A.x = b
function x = ResolutionGauss(A, b)
    [At, bt] = trigGauss(A, b)
    x = solsup(At, bt)
endfunction
