from cyclops import *
from omega import *
moduleEnableCyclops()
sceneLoad("cyclops/test/dna.scene")
lightSetPosition(1, 5, 2, 1)
camera = cameraGetDefault()
cameraEnableFreeFly(camera)

