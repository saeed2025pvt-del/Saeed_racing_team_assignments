import csv
import pandas as pd
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation

#making arrays with corresponding coords

df=pd.read_csv('cones.csv')
maskB=df['color']=='blue'
bluerows=df[maskB]
blue=bluerows[['x','y']].to_numpy()  #final blue array



maskY=df['color']=='yellow'
yellowrows=df[maskY]
yellow=yellowrows[['x','y']].to_numpy()   #final yellow array

cones=np.vstack((blue,yellow))            #all cones    or we could use df[["x", "y"]].to_numpy()

center=cones.mean(axis=0)


#sorting by angles

def sorting(points, center):
    angles = np.arctan2(points[:,1] - center[1],points[:,0] - center[0])
    order = np.argsort(angles)
    return points[order]

sortedB = sorting(blue, center)
sortedY = sorting(yellow, center)


#middle line

openpath=(sortedB+sortedY)/2
closedpath=np.vstack((openpath,openpath[0]))   #closing loop



#function to update coords
fig, ax = plt.subplots()
car, = ax.plot([], [], 'ro', markersize=8)
x= closedpath[:,0]
y = closedpath[:,1]
ax.set_xlim(min(x)-1, max(x)+1)
ax.set_ylim(min(y)-1, max(y)+1)

def animate(frame):
    x, y = closedpath[frame]
    car.set_data([x], [y])   
    return car,

ani = animation.FuncAnimation(fig, animate, frames=len(closedpath), interval=50, blit=True)

#plotting 


plt.figure(figsize=(8,8))
plt.plot(closedpath[:,0], closedpath[:,1], color='black', linewidth=3, label="Centerline")
plt.scatter(sortedB[:,0], sortedB[:,1], c='blue', label="Blue cones",marker='^')
plt.scatter(sortedY[:,0], sortedY[:,1], c='#8B8000', label="Yellow cones",marker='^')
plt.axis('equal')
plt.grid()
plt.legend()
plt.title("TRack")
plt.show()

