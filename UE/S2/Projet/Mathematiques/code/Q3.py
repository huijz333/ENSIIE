"""
"	une fonction qui calcule des coefficients binomiaux plus
"	rapidement qu'avec l'approche naive consistant
"	a calculer des factoriels
"""
def C(n, k):
	if 0 <= k <= n:
		ni = 1
		ki = 1
		for t in range(1, min(k, n - k) + 1):
			ni *= n
			ki *= t
			n -= 1
		return ni // ki
	return 0

"""
"	question 3
"""
def price1(N, rN, hN, bN, s, f):
	qN = (rN - bN) / float(hN - bN)
	return 1.0 / (1.0 + rN)**N * sum([C(N, k) * qN**k * (1.0 - qN)**(N - k)
					* f((1.0 + hN)**k * (1.0 + bN)**(N - k) * s)
					for k in range(0, N + 1)
					])

