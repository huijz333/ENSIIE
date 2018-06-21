"""
"	question 12 : price 3 (o <=> sigma)
"""
from math import exp
from math import sqrt
from numpy.random import normal as rnorm

def price3(n, s, r, o, T, f):
	return 1.0 / n * exp(-r * T) * sum([f(s * exp((r - o*o/2.0)*T + o * sqrt(T) * ksi)) for ksi in rnorm(0.0, 1.0, n)])
