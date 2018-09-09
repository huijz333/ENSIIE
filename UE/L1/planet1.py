# import des dependances (cos, sin, arctan et sqrt)
import math
import matplotlib.pyplot as plt
import numpy as np

# l'animation a ete inspire de:
# https://stackoverflow.com/questions/10896054/simple-animation-of-2d-coordinates-using-matplotlib-and-pyplot

# pas d'integration (en secondes)
dt = 0.01

# nombre de FPS de l'animation
FPS = 60

# constantes physiques
G  = 6.674 * 10e-11
Ms = 1.989 * 10e30

# liste des planetes, format: [nom, masse, x, y, vx, vy, ax, ay]
# (a l'instant t0)
planetes =	[
			# conditions initiales de la Terre:
			# angle par rapport au soleil de 0
			# la vitesse de la Terre est d'environ 30km.s-1
			# la distance au Soleil est d'environ 149.6 millards de m
	 		["Terre",  5.972*10e24, 1.496*10e11, 0.0, 0.0, 30.0*10e3, 0.0, 0.0],
	 		["Mars",   6.419*10e23, 2.066*10e11, 0.0, 0.0, 40.0*10e3, 0.0, 0.0]
		]
# constantes correspond aux index dans le tableau
NOM	= 0
MASSE	= 1
X 	= 2
Y	= 3
VX	= 4
VY	= 5
AX	= 6
AY	= 7

# pyplot initialisation
(fig, ax) = plt.subplots()
xs = [planete[X] for planete in planetes]
ys = [planete[Y] for planete in planetes]
points, = ax.plot(xs, ys, marker='o', linestyle='None')
ax.set_xlim(-1.0, 1.0)
ax.set_ylim(-1.0, 1.0)

# integration infini
while True:
	# on integre N fois avant d'afficher
	N = 100000
	# (on passe alors en accelerer N * dt secondes)
	for i in xrange(0, N):
		for planete in planetes:
			# coordonnees polaires
			d     = math.sqrt(planete[X]**2 + planete[Y]**2)
			theta = math.atan2(planete[Y], planete[X])
		
			# met a jour l'acceleration a partir de l'equation differentielle:
			# M * A = F
			# (on a simplifier la masse)
			F  = -G * Ms / (d * d)
		
			# on projette sur les 2 axes
			planete[AX] = F * math.cos(theta)
			planete[AY] = F * math.sin(theta)

			# met a jour la vitesse (v = a * dt)
			planete[VX] = planete[VX] + dt * planete[AX]
			planete[VY] = planete[VY] + dt * planete[AY]
	
			# met a jour la position
			planete[X] = planete[X] + dt * planete[VX]
			planete[Y] = planete[Y] + dt * planete[VY]
	# on afffiche
	# distances du soleil

	# position relativement au soleil, mis a l'echelle [-1.0, 1.0]
	MX = 4.0*10e11 # (voir Aphelie)
	MY = 1.47*10e11 # (voir Perilie)
	xs = np.array([planete[X] / MX for planete in planetes])
	ys = np.array([planete[Y] / MY for planete in planetes])
	print(xs, ys)
	# on plot
	points.set_data(xs, ys)
	plt.pause(0.01)
