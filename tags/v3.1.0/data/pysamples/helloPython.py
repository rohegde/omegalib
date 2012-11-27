from math import *
from euclid import *
from omega import *
from cyclops import *
#from omegaToolkit import *

sphere = SphereShape.create(1, 4)
sphere.setPosition(Vector3(0, 2, -5))
sphere.setEffect("colored -d green")

plane = PlaneShape.create(10, 10)
plane.setPosition(Vector3(0, 0, -5))
plane.pitch(radians(-90))
plane.setEffect("colored -d gray")

light = Light.create()
light.setColor(Color("white"))
light.setAmbient(Color("#202020"))
light.setEnabled(True)

lightSphere = SphereShape.create(0.1, 4)
lightSphere.setPosition(Vector3(0, 0, 0))
lightSphere.setEffect("colored -d yellow")
lightSphere.setPosition(Vector3(0, 5, -5))

interactor = ToolkitUtils.setupInteractor("config/interactor")
interactor.setSceneNode(lightSphere)
lightSphere.addChild(light)

scene = getSceneManager()
scene.setMainLight(light)

torusModel = ModelInfo()
torusModel.name = "torus"
torusModel.path = "cyclops/test/torus.fbx"
torusModel.size = 3.0
scene.loadModel(torusModel)

torus = StaticObject.create("torus")
torus.setPosition(Vector3(0, 4, -4.5))
torus.setEffect("colored -d blue")
torus.pitch(radians(20))
torus.roll(radians(20))


def onUpdate(frame, t, dt):
    torus.setPosition(Vector3(0, sin(t) * 0.5 + 0.5, -2))
	# torus.pitch(dt)
	# torus.yaw(dt)

def onEvent():
	e = getEvent()
	print(e.getType())

setUpdateFunction(onUpdate)
#setEventFunction(onEvent)

# def onSelectedChanged(source):
    # if isSelected(source):
        # setEffect(source, "colored -d yellow")
    # else:
        # setEffect(source, "colored -d green")


# loadModel("simpleModel", "cyclops/test/torus.fbx", 1.0)

# object = newStaticObject("simpleModel")
# setEffect(object, "colored -d green")

# plane = newPlaneShape(4, 4, 1, 1)
# setEffect(plane, "colored -d gray")
# pitch(plane, radians(-90))
# setPosition(plane, 0, -1, -2)

# light = newLight()
# setLightEnabled(light, True)
# setMainLight(light)
# setPosition(light, 0, 50, 0)
# setLightColor(light, "#ffff90")
# setLightAmbient(light, "#202020")

# addToEditor(object)
# setEditorEnabled(True)

# addSelectionListener(object, "onSelectedChanged(object)")

# menu = newMenu("menu")
# setMainMenu(menu)

# quitMenuItem = addMenuItem(menu, ButtonMenuItem)
# setMenuItemText(quitMenuItem, "Quit")
# setMenuItemCommand(quitMenuItem, "oexit()")






