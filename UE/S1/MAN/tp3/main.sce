funcprot(0) //reset des fonctions precedemment declarés
exec("/home/romain.pereira/MAN/tp3/hilbert.sci")
exec("/home/romain.pereira/MAN/tp1/inverse.sci")

//  1
//  1.a)
H = hilbert(3)
disp(H)

// 1.b)

function [N] = norm2(A)
    N = max(abs(spec(A * conj(A)')))
endfunction

disp("conditionnement matrice hilbert: ")
for n = 1:10
    H = hilbert(n)
    H1 = inverse(H)
    C = norm2(H) * norm2(H1)
    disp(n)
    disp(C)
end

//  2
A = [10 7 8 7 ; 7 5 6 5 ; 8 6 10 9 ; 7 5 9 10]
b = [32 ; 23 ; 33 ; 31]

//  2.a)
if (det(A) ~= 0) then
    A1 = inverse(A)
end

//  2.b) linsolve() résout Ax + b = 0, d'où le '-' devant b
[x, kerA] = linsolve(A, -b)
disp(norm(A * x - b))

//  2.c)
dA = [0 0 0.1 0.2 ; 0.08 0.04 0 0 ; 0 -0.02 -0.02 0 ; -0.01 -0.01 0 -0.02]
db = [0.1 ; -0.1 ; 0.1 ; -0.1]
[x0, kerA] = linsolve(A, -b - db)
disp(norm(A * x0 - b - db))

[x1, kerAdA] = linsolve(A + dA, -b)
disp(norm((A + dA) * x1 - b))

//  inégalités vu en cours
