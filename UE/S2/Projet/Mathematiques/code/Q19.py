import matplotlib.pyplot as plt
from Q3 import price1
from Q15 import put
from math import exp
from math import sqrt

"""
"	question 19
"""
s = 100.0
o = 0.2
r = 0.04
T = 1.0
f = lambda x : max(100 - x, 0)

# pricer 1
X = [10 * k for k in range(1, 100 + 1)]
Y = []
for N in X:
	rN = r * T / N
	hN = (1.0 + rN) * exp( o * sqrt(T / N)) - 1.0
	bN = (1.0 + rN) * exp(-o * sqrt(T / N)) - 1.0
	Y.append(price1(N, rN, hN, bN, s, f))

plt.plot(X, Y)

# put
PX = [X[0], X[-1]]
p  = put(s, r, o, T, 100.0)
PY = [p, p]
plt.plot(PX, PY, "r")

# show
plt.title("price1 et put")
plt.xlabel("N")
plt.ylabel("prix")
plt.legend(["price1", "put"])

plt.show()
