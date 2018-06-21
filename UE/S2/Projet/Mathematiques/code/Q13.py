"""
"	question 13 : tracer courbe price3
"""

from Q12 import price3
import matplotlib.pyplot as plt

N = 10
x = [100000 * k for k in range(1, N + 1)]
y = [price3(n, 100.0, 0.03, 0.1, 1.0, lambda x: max(100.0 - x, 0)) for n in x]
ax = plt.plot(x, y)
plt.title("Resultat du price3 en fonction de n : \n le cardinal d'une distribution N(0, 1)")
plt.xlabel("n")
plt.ylabel("price3")
plt.legend(["price3"])
plt.show()
