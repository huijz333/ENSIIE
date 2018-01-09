funcprot(0) //reset des fonctions precedemment declarés
exec("/home/romain.pereira/MAN/tp4/iter.sci")
exec("/home/romain.pereira/MAN/tp4/decomp.sci")
exec("/home/romain.pereira/MAN/tp4/iterjac.sci")
exec("/home/romain.pereira/MAN/tp4/iterGS.sci")
exec("/home/romain.pereira/MAN/tp4/iterSOR.sci")
//  1
A = [1 2 2 1 ; -1 2 1 0 ; 0 1 -2 2 ; 1 2 1 2]
b = [ 1 ; 1 ; 1 ; 1]

////  1.a) (voir iter.sci)
////  1.b)
//B = eye(4) - A
//b = b
//x0 = b
//n = 20
//xn = iter(B, b, x0, n)  // x(20)
//
//
////  2.
////  2.a) (voir decomp.sci)
////  2.b) (voir jac.sci, GS.sci)
////  2.c) (voir iterJAC.sci, iterGS.sci)
//xn = iterjac(A, b, 10)
//xn = iterGS(A, b, 10)
//xn = iterjac(A, b, 20)
//xn = iterGS(A, b, 20)
//xn = iterjac(A, b, 30)
//xn = iterGS(A, b, 30)
//disp(norm(A * xn - b))
//
//xn = iterSOR(A, b, 30, 1)
//disp(norm(A * xn - b))
//
//xn = iterSOR(A, b, 30, 0.5)
//disp(norm(A * xn - b))
//
//xn = iterSOR(A, b, 30, 1.5)
//disp(norm(A * xn - b))
//
//xn = iterSOR(A, b, 30, 0.000001)
//disp(norm(A * xn - b))
//
//xn = iterSOR(A, b, 30, 0.98)
//disp(norm(A * xn - b))

//  3.
exec("/home/romain.pereira/MAN/tp4/iterGStest.sci")
A = A
b = b
x0 = b
n = 500
eps = 1e-05
[xn, n] = iterGStest(A, b, x0, n, eps)
disp(norm(A * xn - b))

exec("/home/romain.pereira/MAN/tp4/iterSORtest.sci")
A = A
b = b
x0 = b
n = 500
eps = 1e-05
w = 0.98
[xn, n] = iterSORtest(A, b, x0, n, eps, w)
disp(norm(A * xn - b))


