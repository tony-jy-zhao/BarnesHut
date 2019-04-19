import numpy as np
import os
import matplotlib.pyplot as plt

fpath = r"../BarnesHut/"
fname = r"diffBtwTwo.txt"

print(os.getcwd())

with open(fpath+fname) as f:
    lines = f.readlines()
    x = [float(line.strip()) for line in lines]

plt.plot(x, 'o')

ax = plt.gca()



plt.show()

fpath = r"../BarnesHut/"
fname = r"dev.txt"

x2 = []
with open(fpath+fname) as f:
    lines = f.readlines()
    for line in lines:
        x2.append([float(item) for item in line.strip().split()])


print(x2)
plt.plot(x2, 'o')

ax = plt.gca()

plt.show()

