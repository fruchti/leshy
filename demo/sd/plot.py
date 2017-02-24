#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt('example-log.csv', delimiter = ',', skip_header = 4, names = ['t', 'T'])

plt.plot(data['t'] / 1e3, data['T'])
plt.ylabel('T/°C')
plt.xlabel('t/s')
plt.show()
