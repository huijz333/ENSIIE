# import des dependances (cos, sin, arctan et sqrt)
import math
import matplotlib.pyplot as plt

# l'animation a ete inspire de:
# https://stackoverflow.com/questions/10896054/simple-animation-of-2d-coordinates-using-matplotlib-and-pyplot

# renvoie le signe d'un nombre
def sign(x):
	if x < 0.0:
		return -1
	if x > 0.0:
		return 1
	return 0

# liste des planetes, format: [nom, distance au soleil, theta 0, vitesse angulaire / au Soleil]
# constantes correspond aux index dans le tableau
NOM	= 0
D	= 1
THETA0	= 2
OMEGA	= 3
planetes =	[
	 		["Terre", 1.5 * 10e11, 0.0, 2.0 * math.pi / (365.0 * 24.0 * 60.0 * 60.0)],
	 		["Mars",  2.3 * 10e11, 0.0, 2.0 * math.pi / (687.0 * 24.0 * 60.0 * 60.0)]
		]
MAX_D = max([planete[D] for planete in planetes]) * 1.2

# index des planetes dans le tableau
TERRE = 0
MARS  = 1

# pyplot initialisation
ax = plt.gca()

# temps ecoules entre 2 images
dt  = 4 * 24.0 * 60.0 * 60.0 # 4 jours
# nombre d'image par secondes
FPS = 120
# animation infini
n = 0
while True:
	# dessin
	
	# effacement
	ax.clear()
	ax.set_xlim(-1.0, 1.0)
	ax.set_ylim(-1.0, 1.0)

	# soleil
	plt.plot(0, 0, marker='o', linestyle='None')
	plt.text(0, 0, "Soleil")

	# met a jour
	for planete in planetes:
		# calcul de l'angle
		O = planete[THETA0] + n * dt * planete[OMEGA]
		# position reel
		X = planete[D] * math.cos(O) / MAX_D
		Y = planete[D] * math.sin(O) / MAX_D
		# position logarithmique
		x = sign(X) * math.log(abs(X) + 1.0) / math.log(2.0)
		y = sign(Y) * math.log(abs(Y) + 1.0) / math.log(2.0)
		plt.plot(x, y, marker='o', linestyle='None')
		plt.text(x, y, planete[NOM])
	
	plt.legend(["Soleil"] + [planete[NOM] for planete in planetes])

	# pause
	plt.pause(1.0 / FPS)

	# temps suivant
	n = n + 1
