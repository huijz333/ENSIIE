#!/usr/bin/env python
# -*- coding: utf-8 -*-
# File              : code/price1.py
# Author            : Romain PEREIRA <romain.pereira@ensiie.fr>
# Date              :38 CET
# Last Modified Date: sam. 17 févr. 2018 15:17:06 CET
# Last Modified By  : Romain PEREIRA <romain.pereira@ensiie.fr>

import numpy.random.binomial as binom

def price1(N, r, h, b, s, f):
	q = (r - b) / (h - b)
	return 1 / (1 + r)**N) + sum(binom(N, k) * q**k * (1 - q)**(N - k) * f((1 + h)**k * (1 + b)**k) for k in xrange(0, N + 1))


def f(x):
	return 1

h = 3
b = 1
r = 2
N = 0
s = 42

print(price1(N, r, h, b, s, f))
