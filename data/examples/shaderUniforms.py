# Basic example showing how to add a headlight to the camera.
from math import *
from euclid import *
from omega import *
from cyclops import *

plane = PlaneShape.create(5, 5)
plane.setPosition(Vector3(0, 0, -5))
plane.setEffect("textured -v emissive -d examples/data/GradientBackground.jpg")
material = plane.getMaterial()

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



