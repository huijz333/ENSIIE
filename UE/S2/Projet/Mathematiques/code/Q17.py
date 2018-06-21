"""
"	question 17
"""
from Q12 import price3
from Q15 import put
import matplotlib.pyplot as plt

# price3
N = 10
X = [100000 * k for k in range(1, N + 1)]
Y = [price3(n, 100.0, 0.03, 0.1, 1.0, lambda x : max(100.0 - x, 0.0)) for n in X]
plt.plot(X, Y)

# put
px = [X[0], X[-1]]
p  = put(100.0, 0.03, 0.1, 1.0, 100.0)
py = [p, p]
plt.plot(px, py, "r")

# show
plt.title("price3 et put")
plt.xlabel("n")
plt.ylabel("prix")
plt.legend(["price3", "put"])

plt.show()

