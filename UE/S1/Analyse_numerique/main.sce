
funcprot(0) //reset des fonctions precedemment declarés
exec("/home/romain.pereira/Analyse_numerique/solinf.sci")
exec("/home/romain.pereira/Analyse_numerique/solsup.sci")
exec("/home/romain.pereira/Analyse_numerique/trigGauss.sci")
exec("/home/romain.pereira/Analyse_numerique/ResolutionGauss.sci")
exec("/home/romain.pereira/Analyse_numerique/FactLU.sci")
exec("/home/romain.pereira/Analyse_numerique/ResolutionLU.sci")
exec("/home/romain.pereira/Analyse_numerique/inverse.sci")

//Applications numeriques:
//  1. Systeme triangulaire
//      a)
L = [1 0 0 ; 2 3 0 ; 1 4 -1]
b = [1 ; 8 ; 10]
x = solinf(L, b)
disp("solinf")
disp(x)
disp(norm(L*x - b))

//      b)
U = [1 2 3 ; 0 4 8 ; 0 0 5]
b = [6 ; 16 ; 15]
x = solsup(U, b)
disp("solsup")
disp(x)
disp(norm(U*x - b))


//  2. Elimination de Gauss
//      a
A = [3 1 2 ; 3 2 6 ; 6 1 -1]
b = [2 ; 1 ; 4]
[At, bt] = trigGauss(A, b)
x = solsup(At, bt)
disp("trigGauss + solsup")
disp(norm(At * x - bt))
disp(norm(A * x - b))

//  b
A = [1 2 3 ; 5 2 1 ; 3 -1 1]
b = [5 ; 5 ; 6]
x = ResolutionGauss(A, b)
disp("ResolutionGauss")
disp(norm(A * x - b))

//  3. Factorisation LU
//      a
disp("FactLU")
A = [3 1 2 ; 3 2 6 ; 6 1 -1]
[L, U] = FactLU(A)
disp(norm(A - L * U))

//      b
disp("ResolutionLU")
A = [1 2 3 ; 5 2 1 ; 3 -1 1]
b = [5 ; 5 ; 6]
x = ResolutionLU(A, b)
disp(norm(A * x - b))

//  4. inverse de matrice
A = [1 2 3 ; 5 2 1 ; 3 -1 1]
B = inverse(A)
disp(A * B)






