#!/usr/bin/python3

""" C++ plotter """

# -------------------------------------------------------------------------------------------------------------------- #
# Importing general packages
# -------------------------------------------------------------------------------------------------------------------- #
import sys
import os
import time
import pdb
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt



# -------------------------------------------------------------------------------------------------------------------- #
# Create the figure
# -------------------------------------------------------------------------------------------------------------------- #

# Load the B-Spline law
u, f = np.loadtxt("output/bspline_law.csv", delimiter=',').transpose()

# Create the figure
fig = plt.figure(figsize=(6, 5))
ax = fig.add_subplot(111)
fontsize = 12
ax.set_xlabel('$x$ axis', fontsize=12, color='k', labelpad=12)
ax.set_ylabel('$y$ axis', fontsize=12, color='k', labelpad=12)
for t in ax.xaxis.get_major_ticks(): t.label.set_fontsize(10)
for t in ax.yaxis.get_major_ticks(): t.label.set_fontsize(10)
# ax.set_xticks([])
# ax.set_yticks([])
# ax.axis('off')

# Draw line plot
line, = ax.plot(u, f)
line.set_linewidth(1.25)
line.set_linestyle("-")
line.set_color("b")
line.set_marker(" ")
line.set_markersize(3.5)
line.set_markeredgewidth(1)
line.set_markeredgecolor("k")
line.set_markerfacecolor("w")
line.set_label('')

# Create legend
# ax.legend(ncol=1, loc='upper right', fontsize=13, edgecolor='k', framealpha=1.0)

# # Set the aspect ratio of the data
# ax.set_aspect(1.0)

# Set the aspect ratio of the figure
ratio = 1.00
x1, x2 = ax.get_xlim()
y1, y2 = ax.get_ylim()
ax.set_aspect(np.abs((x2-x1)/(y2-y1))*ratio)

# Adjust PAD
plt.tight_layout(pad=5.0, w_pad=None, h_pad=None)

# Show the figure
plt.show()


