from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import pandas as pd

fname = r"C:\Users\enduser\Desktop\cvbp.csv"
data = pd.read_csv(fname, header=None)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(data[0], data[1], data[2])

plt.show()