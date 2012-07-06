from math import *
from omega import *
from cyclops import *
from omegaToolkit import *

sphere = SphereShape.create(1, 4)
sphere.setPosition((0, 2, -5))
sphere.setEffect("colored -d green")

plane = PlaneShape.create(10, 10)
plane.setPosition((0, 0, -5))
plane.pitch(radians(-90))
plane.setEffect("colored -d gray")

light = Light.create()
light.setColor(Color("white"))
light.setPosition((0, 50, -5))
light.setEnabled(True)

scene = getSceneManager()
scene.setMainLight(light)

torusModel = ModelInfo()
torusModel.name = "torus"
torusModel.path = "cyclops/test/torus.fbx"
torusModel.size = 3.0
scene.loadModel(torusModel)

torus = StaticObject.create("torus")
torus.setPosition((0, 4, -4.5))
torus.setEffect("colored -d blue")
torus.pitch(radians(20))
torus.roll(radians(20))


def onUpdate(frame, t, dt):
    # #setPosition(object, 0, sin(t) * 0.5 + 0.5, -2)
	torus.pitch(dt)
	torus.yaw(dt)

setUpdateFunction(onUpdate)

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






