#!/usr/bin/env python
import numpy as np
import scipy.linalg as la
import matplotlib.pyplot as plt
import math

# nombre de points
M = 1000
N = M

# parametres
K = 100
r = 0.04
o = 0.1 # sigma
T = 1.0
L = 4.0 * K

# discretisation
t  = np.linspace(0, T, N + 1)
S  = np.linspace(0, L, M + 2)
dT = T / float(N)
dS = L / float(M + 1)

"""
"	initialisation de la grille
"
"    t  ^
"       |
"       |
"       |
"       |
"       |
"       |
"       |
"	---------------->
"                       S
"""

"""
"	@args O : theta, parametre du modele d'integration a utilise
"""
def solve(O):
	# coefficient recurrent dans l'equation
	So = [(o * S[j] / dS) * (o * S[j] / dS) for j in xrange(M + 2)]
	Sr = [r * S[j] / dS for j in xrange(M + 2)]

	# generation d'une matrice A', qui correspond a la matrice A
	# mais bien formatte pour la fonction la.solve_banded
	A = np.zeros((3, M + 2))
	# on genere 'B' avec la meme convention pour optimiser l'espace memoire
	B = np.zeros((3, M + 2))
	# bords
	A[1, 0] = 1.0
	A[1, M + 1] = 1.0
	# 1ere ligne (upper diagonale de A)
	for j in xrange(0, M + 1):
		A[0, j + 1] = -(Sr[j] + 0.5 * So[j]) * (1.0 - O)
		B[0, j + 1] =  (Sr[j] + 0.5 * So[j]) * O
	# 2eme ligne (diagonale de A)
	for j in xrange(1, M + 1):
		A[1, j]     =      1.0 / dT + (Sr[j] + So[j]) * (1.0 - O)
		B[1, j]     = -r + 1.0 / dT - (Sr[j] + So[j]) * O
	# 3eme ligne (lower diagonale de A)
	for j in xrange(1, M + 2):
		A[2, j - 1] = -0.5 * So[j] * (1.0 - O)
		B[2, j - 1] =  0.5 * So[j] * O
	# allocation memoire de C(i), qui sera reconstruite pour chaque iteration
	bi = np.zeros(M + 2)
	# X(M + 1) connu
	Xi = np.array([max(K - S[j], 0.0) for j in xrange(M + 2)])
	# resolutions successives
	for i in xrange(N-1, -1, -1):
		# calcul de b(i)
		# aux bords
		bi[0]      = A[0, 0]     * Xi[0]     + K * np.exp(r * (t[i] - T))
		bi[M + 1]  = A[1, M + 1] * Xi[M + 1] + 0
		# les coefficients du milieu
		for j in range(1, M + 1):
			bi[j] = B[0, j - 1] * Xi[j - 1] + B[1, j] * Xi[j] + B[2, j + 1] * Xi[j + 1]
		
		# resolution de A * X(i) = b(i)
		Xi = la.solve_banded((1, 1), A, bi)
	return Xi

# resout les N systemes, et affiche le prix du put a t=0
# renvoie P(t=0)
def solveAndPlot(theta, title):
	P = solve(theta)
	plt.plot(S, P)
	return P

title = "Resolution implicit et Crank-Nicholson"
plt.figure(num=title)
plt.title(title)
plt.suptitle("N=" + str(N) + " ; M=" + str(M))
plt.xlabel("S")
plt.ylabel("Prix du put P(t=0, S)")

P0 = solveAndPlot(0.0, "implicit")
P1 = solveAndPlot(0.5, "Crank Nicholson")

plt.legend(["implicit", "Crank Nicholson", "explicit"])

# erreurs relatives
print("erreurs relatives:")
N0 = np.linalg.norm(P0)
N1 = np.linalg.norm(P1)

E01 = 2 * np.linalg.norm(P0 - P1) / (N0 + N1)

print("Entre 0.0 et 0.5 (implicit et Crank Nicholson): " + str(E01))

plt.show()
