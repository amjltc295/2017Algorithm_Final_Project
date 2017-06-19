from __future__ import division
import sys
import re
import math
import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from pylab import *

COLOR_A = 0
COLOR_B = 1
COLOR_N = 2
Inf=210000000

#this program is used to plot the balanced output file

class Window:
    def __init__(self, x_bl, y_bl, x_tr, y_tr):
        self.x_bl = x_bl
        self.y_bl = y_bl
        self.x_tr = x_tr
        self.y_tr = y_tr
        self.area = (x_tr - x_bl) * (y_tr - y_bl)
        if self.area <= 0:
            print ('WARNING!!! Your window\'s area <= 0')

class Shape:
    def __init__(self, x_bl, y_bl, x_tr, y_tr, color):
        self.x_bl = x_bl
        self.y_bl = y_bl
        self.x_tr = x_tr
        self.y_tr = y_tr
        self.color = color
        self.area = (x_tr - x_bl) * (y_tr - y_bl)
        if self.area <= 0:
            print ('WARNING!!! Your shape\'s area <= 0')

if len(sys.argv) < 2:
    print ('Usage: python plot_balanced.py <output file from your program> <output picture file>')
    sys.exit()

win_num = 0
windows_list = [] 
shapes_list = []
shape_parsing = False
minx=Inf
maxx=-Inf
miny=Inf
maxy=-Inf
with open (sys.argv[1]) as f:
    for line in f:
        if re.search('[a-zA-Z0-9]+',line).group().lower() == 'group':
            shape_parsing = True
            continue
        if shape_parsing:
            line = line.replace('=',',').split(',')
            if line[0].split('[')[0].lower() == 'ca':
                color = COLOR_A
            elif line[0].split('[')[0].lower() == 'cb':
                color = COLOR_B
            else:
                color = COLOR_N
            points = [int(num) for num in line[1:]]
            minx=min(minx,points[0])
            maxx=max(maxx,points[2])
            miny=min(miny,points[1])
            maxy=max(maxy,points[3])
            shape = Shape(points[0],points[1],points[2],points[3],color)
            shapes_list.append(shape)
            continue
        line = line.split('(')
        win = line[0].replace('=',',').split(',')
        points = [int(num) for num in win[1:]]
        window = Window(points[0],points[1],points[2],points[3])
        windows_list.append(window)
        win_num += 1

"""
###for debug usage, print the windows and shapes
i=0
for window in windows_list:
    i+=1
    print(i,',',window.x_bl,',',window.y_bl,',',window.x_tr,',',window.y_tr)
i=0
for shape in shapes_list:
    i+=1
    print(i,',',shape.x_bl,',',shape.y_bl,',',shape.x_tr,',',shape.y_tr,',',shape.color)

"""

fig=plt.figure(figsize=(32,18))
ax1 = fig.add_subplot(111, aspect='equal')
for window in windows_list:
    ax1.add_patch(patches.Rectangle((window.x_bl,window.y_bl),window.x_tr-window.x_bl,window.y_tr-window.y_bl,linewidth=1,edgecolor='b',facecolor='none'))
for shape in shapes_list:
    if shape.color==0:
        ax1.add_patch(patches.Rectangle((shape.x_bl,shape.y_bl),shape.x_tr-shape.x_bl,shape.y_tr-shape.y_bl,color='red'))
    elif shape.color==1:
        ax1.add_patch(patches.Rectangle((shape.x_bl,shape.y_bl),shape.x_tr-shape.x_bl,shape.y_tr-shape.y_bl,color='green'))
    elif shape.color==2:
        ax1.add_patch(patches.Rectangle((shape.x_bl,shape.y_bl),shape.x_tr-shape.x_bl,shape.y_tr-shape.y_bl,color='gray'))
xlim(minx,maxx)
ylim(miny,maxy)
# plt.show()
fig.savefig(sys.argv[2])
plt.close()