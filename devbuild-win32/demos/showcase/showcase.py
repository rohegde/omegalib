from math import *
from omega import *
from cyclops import *
from omegaToolkit import *

scene = getSceneManager()
scene.createSkyBox("demos/common/brightday1", "png")

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
model.path = "terrance/data/airplane.obj"
model.size = 5.0
scene.loadModel(model)

model2 = ModelInfo()
model2.name = "model2"
model2.path = "demos/showcase/data/walker/walker.fbx"
model2.size = 5.0
scene.loadModel(model2)

object = StaticObject.create("model")
object.setPosition(Vector3(0, 2.5, -5))
object.setEffect("colored -d gray")

object2 = StaticObject.create("model2")
object2.setPosition(Vector3(5, 0, -5))
object2.setEffect("colored -d gray")
object2.yaw(-3.1415/2.0)

def onUpdate(frame, t, dt):
	object.yaw(dt)
	

setUpdateFunction(onUpdate)







