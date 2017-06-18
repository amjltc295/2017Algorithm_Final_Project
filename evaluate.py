from __future__ import division
import sys
import re
import math

COLOR_A = 0
COLOR_B = 1

def equal(shape1,shape2):
    if shape1.x_bl == shape2.x_bl and shape1.y_bl == shape2.y_bl and\
       shape1.x_tr == shape2.x_tr and shape1.y_tr == shape2.y_tr:
        return True
    else:
        return False

def violation(shape1,shape2,alpha,beta):
    if shape1.x_bl >= shape2.x_bl and shape1.x_bl < shape2.x_tr:
        if abs(shape1.y_bl - shape2.y_tr) < beta and shape1.color == shape2.color:
            return True
        if abs(shape1.y_tr - shape2.y_bl) < beta and shape1.color == shape2.color:
            return True 
    elif shape1.x_tr > shape2.x_bl and shape1.x_tr < shape2.x_tr:
        if abs(shape1.y_bl - shape2.y_tr) < beta and shape1.color == shape2.color:
            return True
        if abs(shape1.y_tr - shape2.y_bl) < beta and shape1.color == shape2.color:
            return True 

    if shape1.y_bl >= shape2.y_bl and shape1.y_bl < shape2.y_tr:
        if abs(shape1.x_bl - shape2.x_tr) < alpha and shape1.color == shape2.color:
            return True
        if abs(shape1.x_tr - shape2.x_bl) < alpha and shape1.color == shape2.color:
            return True 
    elif shape1.y_tr > shape2.y_bl and shape1.y_tr < shape2.y_tr:
        if abs(shape1.x_bl - shape2.x_tr) < alpha and shape1.color == shape2.color:
            return True
        if abs(shape1.x_tr - shape2.x_bl) < alpha and shape1.color == shape2.color:
            return True 

class Window:
    def __init__(self, x_bl, y_bl, x_tr, y_tr, ca, cb):
        self.x_bl = x_bl
        self.y_bl = y_bl
        self.x_tr = x_tr
        self.y_tr = y_tr
        self.area = (x_tr - x_bl) * (y_tr - y_bl)
        if self.area <= 0:
            print 'WARNING!!! Your window\'s area <= 0'
        self.ca = ca
        self.cb = cb
        self.ca2 = 0
        self.cb2 = 0
    def intersection(self,shape):
        inter_area = max(0, min(self.x_tr, shape.x_tr) - max(self.x_bl, shape.x_bl)) *\
                     max(0, min(self.y_tr, shape.y_tr) - max(self.y_bl, shape.y_bl))
        if shape.color == COLOR_A:
            self.ca2 += inter_area/self.area * 100
        elif shape.color == COLOR_B:
            self.cb2 += inter_area/self.area * 100
        #print shape.color,shape.area,inter_area
class Shape:
    def __init__(self, x_bl, y_bl, x_tr, y_tr, color):
        self.x_bl = x_bl
        self.y_bl = y_bl
        self.x_tr = x_tr
        self.y_tr = y_tr
        self.color = color
        self.area = (x_tr - x_bl) * (y_tr - y_bl)
        if self.area <= 0:
            print 'WARNING!!! Your shape\'s area <= 0'
if len(sys.argv) < 2:
    print 'Usage: python evaluate.py <output file from your program>'
    print 'Usage: python evaluate.py <output file from your program> <alpha> <beta>'
    sys.exit()

win_num = 0
windows_list = [] 
shapes_list = []
score = 30
temp_score = []
shape_parsing = False
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
                continue
            points = [int(num) for num in line[1:]]
            shape = Shape(points[0],points[1],points[2],points[3],color)
            shapes_list.append(shape)
            continue
        line = line.split('(')
        win = line[0].replace('=',',').split(',')
        points = [int(num) for num in win[1:]]
        den = line[1].replace(')','').split()
        den = [float(num) for num in den]
        window = Window(points[0],points[1],points[2],points[3],den[0],den[1])
        windows_list.append(window)
        temp_score.append( abs(den[1]-den[0])/5 )
        win_num += 1
for i in range(win_num):
    score  += 70/win_num - temp_score[i]

print 'window number:', win_num
print 'score calculated by color density:', score

win_num = len(windows_list)
temp_score = []
score = 30
for window in windows_list:
    for shape in shapes_list:
        window.intersection(shape)
    #print window.ca2, window.cb2
    temp_score.append( abs(window.ca2-window.cb2)/5 )
for i in range(win_num):
    score  += 70/win_num - temp_score[i]

#print 'window number:', win_num
print 'score calculated by windows and shapes:', score
if len(sys.argv) == 4:
    alpha = float(sys.argv[2])
    beta = float(sys.argv[3])
    vio = False
    for shape1 in shapes_list:
        for shape2  in shapes_list:
            if not equal(shape1,shape2):
                if violation(shape1,shape2,alpha,beta):
                    vio = True
    if vio == True:
        print 'ERROR!!! Shape\'s color violates the rule!'
    else:
        print 'Congratulations!!! You have passed the color rule test!' 
