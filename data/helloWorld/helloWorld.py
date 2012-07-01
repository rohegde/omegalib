from math import *
from omega import *
from cyclops import *
from omegaToolkit import *

def onUpdate(frame, t, dt):
    #setPosition(object, 0, sin(t) * 0.5 + 0.5, -2)
    pitch(object, dt)
    yaw(object, dt)

def onSelectedChanged(source):
    if isSelected(source):
        setEffect(source, "colored -d yellow")
    else:
        setEffect(source, "colored -d green")

setUpdateFunction(onUpdate)

loadModel("simpleModel", "cyclops/test/torus.fbx", 1.0)

object = newStaticObject("simpleModel")
setEffect(object, "colored -d green")

plane = newPlaneShape(4, 4, 1, 1)
setEffect(plane, "colored -d gray")
pitch(plane, radians(-90))
setPosition(plane, 0, -1, -2)

light = newLight()
setLightEnabled(light, True)
setMainLight(light)
setPosition(light, 0, 50, 0)
setLightColor(light, "#ffff90")
setLightAmbient(light, "#202020")

addToEditor(object)
setEditorEnabled(True)

addSelectionListener(object, "onSelectedChanged(object)")

menu = newMenu("menu")
setMainMenu(menu)

quitMenuItem = addMenuItem(menu, ButtonMenuItem)
setMenuItemText(quitMenuItem, "Quit")
setMenuItemCommand(quitMenuItem, "oexit()")






