from Q3 import price1
from Q5 import price2
from random import randint

"""
"	question 7
"""
f = lambda x : max(x - 100.0, 0.0)
#for N in range(5, 16):
for N in (randint(5, 15),):
	print("comparaison pour N = " + str(N))
	p1 = price1(N, +0.03, +0.05, -0.05, 100.0, f)
	p2 = price2(N, +0.03, +0.05, -0.05, 100.0, f)
	print("price1 = " + str(p1))
	print("price2 = " + str(p2))

