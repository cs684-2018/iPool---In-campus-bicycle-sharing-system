import matplotlib.pyplot as plt, serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui

win = pg.GraphicsWindow()
win.setWindowTitle('Joystick')
p1 = win.addPlot() 
curve1 = p1.plot()

# plt.ion()
ser = serial.Serial('/dev/ttyACM0')

maxX = 127
maxY = 127

# plt.plot([(maxX / 2), (maxX / 2)], [0, (maxY)])

# plt.plot([0, (maxX)], [(maxY / 2), (maxY / 2)])
# plt.pause(0.0001)
print("Axis Ploting................. Done")
prevx = 0
prevy = 0
x = 0
y = 0
Xflag = 0
xflag = 0
Yflag = 0
yflag = 0
xDataFlag = 0
yDataFlag = 0
plotFlag = 0
counter = 0
print("Variable Init ..............Done")
while 1:
    i = ser.read(1)
    if i == b'X' and Xflag == 0:
        #print(1)
        xDataFlag = 0
        xflag = 0
        Xflag = 1
    if i == b'x' and Xflag == 1 and xflag == 0:
        #print(2)
        Xflag = 0
        xflag = 1
        xDataFlag = 1
        continue
    if xDataFlag == 1:
        #print(3)
        x = i
        xDataFlag = 0
    if i == b'Y' and Yflag == 0:
        #print(4)
        yDataFlag = 0
        yflag = 0
        Yflag = 1
    if i == b'y' and Yflag == 1 and yflag == 0:
        #print(5)
        Yflag = 0
        yflag = 1
        yDataFlag = 1
        continue
    if yDataFlag == 1:
        #print(6)
        y = i
        yDataFlag = 0
        plotFlag = 1
    if plotFlag == 1:
        #print(7)
        plotFlag = 0
        print("x:", y, " y:", x)
        try:
            tempordx = x
            x = ord(y)
            y = ord(tempordx)
            print("\t", x, " ", y)
            if counter >25:
                # plt.plot(x, y, marker='o')
                # plt.pause(0.00001)
                curve1.setData([x,y], pen=None, symbol='o', symbolPen=None, symbolSize=4, symbolBrush=('b'))
                counter = 0
            counter=counter+1
        except Exception as e:
            x = 0
            y = 0
            Xflag = 0
            xflag = 0
            Yflag = 0
            yflag = 0
            xDataFlag = 0
            yDataFlag = 0
            plotFlag = 0