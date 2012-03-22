from cyclops import *
from omega import *
moduleEnableCyclops()
sceneLoad("cyclops/test/demo.scene")
lightSetPosition(0, 0.5, 0, 1)
camera = cameraGetDefault()
cameraEnableFreeFly(camera)

