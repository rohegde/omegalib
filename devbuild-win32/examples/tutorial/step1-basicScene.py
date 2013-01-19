from math import *
from euclid import *
from omega import *
from cyclops import *

sphere = SphereShape.create(1, 4)
sphere.setPosition(Vector3(-1, 2, -5))
sphere.setEffect("colored -d green")

box = BoxShape.create(1, 1, 1)
box.setPosition(Vector3(1, 2, -5))
box.setEffect("colored -d blue")

light = Light.create()
light.setColor(Color("white"))
light.setAmbient(Color("#202020"))
light.setEnabled(True)