# Basic example showing how to add a headlight to the camera.
from math import *
from euclid import *
from omega import *
from cyclops import *

sphere = SphereShape.create(1, 4)
sphere.setPosition(Vector3(0, 2, -5))
sphere.setEffect("colored -d green")

plane = PlaneShape.create(10, 10)
plane.setPosition(Vector3(0, 0, -5))
plane.pitch(radians(-90))
plane.setEffect("textured -d common/cubemaps/grid1/negz.png")

scene = getSceneManager()

light = Light.create()
light.setColor(Color("#505050"))
light.setAmbient(Color("#202020"))
light.setPosition(Vector3(0, 20, -5))
light.setEnabled(True)
scene.setMainLight(light)

# headlight
headlight = Light.create()
headlight.setColor(Color("white"))
headlight.setEnabled(True)
getDefaultCamera().addChild(headlight)

def onEvent():
	e = getEvent()
	# toggle headlight on and off
	if(e.isKeyDown('l')):
		print("Toggle Light")
		headlight.setEnabled(not headlight.isEnabled())

setEventFunction(onEvent)



