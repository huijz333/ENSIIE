"""
"	question 5
"""
def price2(N, rN, hN, bN, s, f, debug=False):
	q = (rN - bN) / float(hN - bN)
	def v(k, S_k):
		if k == N:
			return f(S_k)
		E = v(k + 1, S_k * (1.0 + hN)) * q + v(k + 1, S_k * (1.0 + bN)) * (1 - q)
		if (debug):
			print("v(" + str(k) + ")(" + str(S_k) + ")=" + str(E))
		return 1.0 / (1.0 + rN) * E 
	return v(0, s)

