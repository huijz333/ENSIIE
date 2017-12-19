funcprot(0) //reset des fonctions precedemment declarés
exec("/home/romain.pereira/MAN/tp3/hilbert.sci")
exec("/home/romain.pereira/MAN/tp1/inverse.sci")

//  2
//  2.a
H = hilbert(3)
disp(H)

// 2.b

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
