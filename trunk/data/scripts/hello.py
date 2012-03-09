from math import *

def update(frame, time, dt):
    root = nodeGetRoot()
    nodeSetPosition(root, [sin(time), 0, 0])

def run():
    print("Hello python!")
    ofuncUpdate(update)

