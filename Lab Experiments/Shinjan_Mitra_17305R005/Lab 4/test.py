from PyQt5 import QtCore, QtWidgets
import pyqtgraph as pg
import numpy as np
import _thread
import matplotlib.pyplot as plt, serial


# plt.ion()
ser = serial.Serial('/dev/ttyACM0')

maxX = 127
maxY = 127
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
plotX=0
plotY=0
# plt.plot([(maxX / 2), (maxX / 2)], [0, (maxY)])

# plt.plot([0, (maxX)], [(maxY / 2), (maxY / 2)])
# plt.pause(0.0001)
def plot_from_serial():
    print("Axis Ploting................. Done")

    print("Variable Init ..............Done")
    global maxX
    global maxY
    global prevx
    global prevy
    global x
    global y
    global Xflag
    global xflag
    global Yflag
    global yflag
    global xDataFlag
    global yDataFlag
    global plotFlag
    global counter
    global plotX
    global plotY

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
                	plotX=x
                	plotY=y
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
class MyWidget(pg.GraphicsWindow):

    def __init__(self, parent=None):
        super().__init__(parent=parent)

        self.mainLayout = QtWidgets.QVBoxLayout()
        self.setLayout(self.mainLayout)

        self.timer = QtCore.QTimer(self)
        self.timer.setInterval(10) # in milliseconds
        self.timer.start()
        self.timer.timeout.connect(self.onNewData)
        _thread.start_new_thread( plot_from_serial,() )
        self.plotItem = self.addPlot(title="Lidar points")
        self.plotItem.setRange(yRange=[0,128])
        self.plotItem.setRange(xRange=[0,128])
        self.plotDataItem = self.plotItem.plot([], pen=None, 
            symbolBrush=(255,0,0), symbolSize=20, symbolPen=None)


    def setData(self, x, y):
        self.plotDataItem.setData([plotX], [plotY])


    def onNewData(self):
        numPoints = 1000  
        x = np.random.normal(size=numPoints)
        y = np.random.normal(size=numPoints)
        self.setData(x, y)


def main():
    app = QtWidgets.QApplication([])

    pg.setConfigOptions(antialias=False) # True seems to work as well

    win = MyWidget()
    win.show()
    win.resize(800,600) 
    win.raise_()
    app.exec_()

if __name__ == "__main__":
    main()