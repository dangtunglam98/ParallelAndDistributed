import sys
import os
import matplotlib as mpl
if os.environ.get('DISPLAY','') == '':
    print('no display found. Using non-interactive Agg backend')
    mpl.use('Agg')
import matplotlib.pyplot as plt

information = []
num_thread = []
exec_time = []
with open(sys.argv[1], 'r') as f:
    data = f.read().splitlines()
    information = data[:4]
    data = data[4:]
    for item in data:
        splitInfo = item.split()
        num_thread.append(int(splitInfo[0]))
        exec_time.append(float(splitInfo[1]))

information[1] =information[1].replace('\t', '')
title = ''
y_pos = 0.8
for item in information[:3]:
    plt.figtext(.7, y_pos, item)
    y_pos -= 0.03
plt.plot(num_thread, exec_time)
plt.ylabel('Wall Time')
plt.xlabel('Number of Threads')
plt.title(information[3])
plt.savefig(information[0][:6]+'_'+information[3][:6]+'.pdf')
