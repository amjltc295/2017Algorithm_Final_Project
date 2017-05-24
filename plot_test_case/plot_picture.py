# -*- coding: utf8 -*-

import numpy as np
import csv,sys
import matplotlib.pyplot as plt
import os
import matplotlib.patches as patches
from pylab import *

# run with 2 inputs, input file and output file
# for example: "python3.5 plot_picture.py case1 plotcase1.png"

text=open(sys.argv[1],'r')
row=csv.reader(text,delimiter=(","))
alpha=0
beta=0
x1=[]
x2=[]
y1=[]
y2=[]
D_row=0
for r in row:
	if D_row==0:
		alpha=r[0]
		alpha=alpha.split('=')
		alpha=int(alpha[1])
	if D_row==1:
		beta=r[0]
		beta=beta.split('=')
		beta=int(beta[1])
	if D_row>2:
		x1.append(int(r[0]))
		y1.append(int(r[1]))
		x2.append(int(r[2]))
		y2.append(int(r[3]))
	D_row+=1
text.close()

minx=min(x1)-alpha
miny=min(y1)-beta
maxx=max(x2)+alpha
maxy=max(y2)+beta

print("minx={} miny={} maxx={} maxy={}".format(minx,miny,maxx,maxy))
print("alpha={} beta={}".format(alpha,beta))


# for i in range(len(x1)):
	# print("x1={} y1={} x2={} y2={}".format(x1[i],y1[i],x2[i],y2[i]))


fig=plt.figure(figsize=(32,18))
ax1 = fig.add_subplot(111, aspect='equal')
for i in range(len(x1)):
	ax1.add_patch(patches.Rectangle((x1[i]-alpha,y1[i]-beta),x2[i]-x1[i]+2*alpha,y2[i]-y1[i]+2*beta,linewidth=1,edgecolor='r',facecolor='none'))
for i in range(len(x1)):
	ax1.add_patch(patches.Rectangle((x1[i],y1[i]),x2[i]-x1[i],y2[i]-y1[i],color='green'))
	ax1.text(x1[i],y1[i],"shape{}".format(i))
xlim(minx,maxx)
ylim(miny,maxy)
# plt.show()
fig.savefig(sys.argv[2])
plt.close()