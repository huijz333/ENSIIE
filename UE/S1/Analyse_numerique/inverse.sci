exec("/home/romain.pereira/Analyse_numerique/ResolutionGauss.sci")


//
//  Algorithme:
//      - on resout A.xi = ei , où ei est le ieme vecteur de la base canonique de R^n
//      - la ieme colonne de B contient xi

//  fonction: 'inverse'
//  parametre:  A : matrice inversible
//  retour:     B : l'inverse de A
function [B] = inverse(A)
    n = size(A, 1)
    I = eye(n, n)
    B = []
    for i = 1:n
        B = cat(2, B, ResolutionGauss(A, I(1:n, i)))
    end
endfunction
