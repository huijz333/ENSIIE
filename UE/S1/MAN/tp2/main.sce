funcprot(0) //reset des fonctions precedemment declarés
exec("/home/romain.pereira/MAN/tp2/Cholesky.sci")
exec("/home/romain.pereira/MAN/tp2/ResolutionCholesky.sci")
exec("/home/romain.pereira/MAN/tp1/ResolutionLU.sci")
exec("/home/romain.pereira/MAN/tp1/solinf.sci")
exec("/home/romain.pereira/MAN/tp1/solsup.sci")
exec("/home/romain.pereira/MAN/tp1/trigGauss.sci")
exec("/home/romain.pereira/MAN/tp1/ResolutionGauss.sci")
exec("/home/romain.pereira/MAN/tp1/FactLU.sci")
exec("/home/romain.pereira/MAN/tp1/ResolutionLU.sci")
exec("/home/romain.pereira/MAN/tp1/inverse.sci")


// 1 (voir cours)

// 2
A = [15 10 18 12 ; 10 15 7 13 ; 18 7 27 7 ; 12 13 7 22]
B = Cholesky(A)
disp(norm(A - B * B'))

// 3
b = [53 ; 72 ; 26 ; 97]
x = ResolutionCholesky(A, b)
disp(norm(A * x - b))

// 4
function [A] = generer(n)
    B = zeros(n, n)
    for i = 2:n
        B(i, i - 1) = 1
    end
    A = 3 * eye(n, n) + B + B'
endfunction

function [t] = test(n, f)
    //genere b
    b = ones(n, 1)

    //genere A
    B = zeros(n, n)
    for i = 2:n
        B(i, i - 1) = 1
    end
    A = 3 * eye(n, n) + B + B'

    //test methode
    tic()
    x = f(A, b)
    t = toc()
endfunction

function [x] = LUCarre(A, b)
    x = ResolutionLU(A * A, b)
endfunction


function [x] = LU(A, b)
    [L, U] = FactLU(A)
    y1 = solinf(L, b)
    y2 = solsup(U, y1)
    y3 = solinf(L, y2)
    x = solsup(U, y3)
endfunction

function [x] = CholeskyCarre(A, b)
    x = ResolutionCholesky(A * A, b)
endfunction

function [x] = CholeskyNormal(A, b)
    tic()
    B = Cholesky(A)
    disp(toc())
    L = B
    U = B'
    y1 = solinf(L, b)
    y2 = solsup(U, y1)
    y3 = solinf(L, y2)
    x = solsup(U, y3)
endfunction

function [ts] = traceCourbe(ns, f, l)
    N = size(ns, 1)
    ts = zeros(N)
    for i=1:N
        ts(i) = test(ns(i), f)
    end
    plot(ns, ts, l)
endfunction

n0 = 150
n1 = 250
step = 10
N = round((n1 - n0) / step)
ns = zeros(N)
for i=1:N
    ns(i) = n0 + i * step
end

traceCourbe(ns, LUCarre, 'r.->')
traceCourbe(ns, LU, 'g.->')
traceCourbe(ns, CholeskyCarre, 'b.->')
traceCourbe(ns, CholeskyNormal, 'm.->')

legend(['LUCarre';'LU' ; 'CholeskyCarre' ; 'CholeskyNormal']);
