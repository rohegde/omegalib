from math import *
from omega import *
from cyclops import *
from omegaToolkit import *

scene = getSceneManager()
scene.createSkyBox("demos/common/brightday1", "png")

plane = PlaneShape.create(50, 50)
plane.setPosition(Vector3(0, 0, -5))
plane.pitch(radians(-90))
plane.setEffect("colored -d #a0a0a0")

# sphere = SphereShape.create(3, 4)
# sphere.setPosition(Vector3(1, 2, -4))
# sphere.setEffect("colored -d gray")


light = Light.create()
light.setColor(Color("white"))
light.setPosition(Vector3(0, 50, -5))
light.setEnabled(True)

scene = getSceneManager()
scene.setMainLight(light)

model = ModelInfo()
model.name = "model"
model.path = "demos/showcase/data/walker/walker.fbx"
model.size = 5.0
scene.loadModel(model)

object = StaticObject.create("model")
object.setPosition(Vector3(0, 0, -5))
object.setEffect("colored -d gray")


def onUpdate(frame, t, dt):
	object.yaw(dt)

setUpdateFunction(onUpdate)







