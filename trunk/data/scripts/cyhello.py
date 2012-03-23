from cyclops import *
from omega import *
moduleEnableCyclops()
sceneLoad("cyclops/test/demo.scene")
lightSetPosition(1, 1, 0, 1)
camera = cameraGetDefault()
cameraEnableFreeFly(camera)

