from __future__ import division
import sys
import re

if len(sys.argv) < 2:
    print 'Usage: python evaluate.py <output file from your program>'
    sys.exit()

win_num = 0 
score = 30
temp_score = []

with open (sys.argv[1]) as f:
    for line in f:
        if re.search('[a-zA-Z0-9]+',line).group().lower() == 'group':
            break
        line = line.split('(')
        den = line[1].replace(')','').split()
        den = [float(num) for num in den]
        temp_score.append( abs(den[1]-den[0])/5 )
        win_num += 1
for i in range(win_num):
    score  += 70/win_num - temp_score[i]

print 'window number:', win_num
print 'score:', score

    
