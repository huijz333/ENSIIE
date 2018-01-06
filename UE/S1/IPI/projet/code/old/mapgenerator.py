import random
import sys

ratio = 0.2
l = int(sys.argv[1])
print(l)
empty = 2
for i in range(l):
	line = ""
	for j in range(l):
		if i == 0 and j == 0:
			c = 'E'
		elif i == l - 1 and j == l - 1:
			c = 'S'
		elif random.randint(0, 100) * 0.01 < ratio:
			c = 'X'
		else:
			empty = empty + 1
			c = '.'
		line = line + c
	print(line)
print(empty)
