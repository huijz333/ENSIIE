from mpl_toolkits.mplot3d.axes3d import *
from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np
from Q15 import put

"""
"	question 18
"""
fig = plt.figure()
ax = Axes3D(fig)
N = 10
X = [20.0 * k for k in range(1, N + 1)]
Y = [ 1.0 / k for k in [12.0, 6.0, 4.0, 3.0, 2.0, 1.0]]
Z = [[put(s, 0.03, 0.1, T, 100.0) for s in X] for T in Y]
X, Y = np.meshgrid(X, Y)
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.jet,linewidth=1, antialiased=True)
ax.set_xlabel("s")
ax.set_ylabel("T")
ax.set_zlabel("put")
plt.title("prix du put en fonction de s et de T")
plt.show()
