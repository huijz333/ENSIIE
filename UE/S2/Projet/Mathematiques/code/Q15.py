from math import sqrt
from math import exp
from math import log
from scipy.stats import norm # distribution function

"""
"	question 15 (o <=> sigma)
"""
def put(s, r, o, T, K):
	d = 1.0 / (o * sqrt(T)) * (log(s / K) + (r + o*o/2) * T)
	F = norm.cdf #cumulative distribution function (fonction repartition)
	return -s * F(-d) + K * exp(-r * T) * F(-d + o * sqrt(T))


