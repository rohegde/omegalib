from math import *
from omega import *
from cyclops import *
from omegaToolkit import *

ground = PlaneShape.create(10, 10)
ground.setPosition((0, 0, -5))
ground.pitch(radians(-90))
ground.setEffect("colored -d gray")

light = Light.create()
light.setColor(Color("white"))
light.setPosition((0, 50, -5))
light.setEnabled(True)

scene = getSceneManager()
scene.setMainLight(light)

planeModel = ModelInfo()
planeModel.name = "plane"
planeModel.path = "terrance/airplane.obj"
planeModel.size = 5.0
scene.loadModel(planeModel)

#sphere = SphereShape.create(3, 4)
#sphere.setPosition((0, 2, -5))
#sphere.setEffect("colored -d green")

plane = StaticObject.create("plane")
plane.setPosition((0, 2, -4))
plane.setEffect("colored -d gray")

def onUpdate(frame, t, dt):
	plane.yaw(dt)

setUpdateFunction(onUpdate)
