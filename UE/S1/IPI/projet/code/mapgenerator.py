import random
import sys

ratio = 0.5
l = int(sys.argv[1])
blocks = []
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
			c = '.'
		line = line + c
	blocks.append(line)



print(l)
for line in blocks:
	print(line)
print(l * l)
