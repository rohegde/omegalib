# startup script for orun
# NOTE: variables that start with `_` are considered system-level variables: they will not be deleted
# during application switches or when calling OmegaViewer.reset(). They will also be accessible from all
# scripts.
from omegaViewer import *

_viewer = getViewer()
_drawer = _viewer.getAppDrawer()

# Set the default scale of the drawer
_drawer.setDrawerScale(2.0)
# Set the default icon size
_drawer.setIconSize(64)

planesApp = AppInfo()
planesApp.name = "./planes.py"
planesApp.label = "Planes"
planesApp.iconFile = "app.png"
_drawer.addApp(planesApp)

multilightApp = AppInfo()
multilightApp.name = "./multilight.py"
multilightApp.label = "Mulilight"
multilightApp.iconFile = "app.png"
_drawer.addApp(multilightApp)

billboardApp = AppInfo()
billboardApp.name = "./billboard.py"
billboardApp.label = "Billboard"
billboardApp.iconFile = "app.png"
_drawer.addApp(billboardApp)

embeddedShaderApp = AppInfo()
embeddedShaderApp.name = "./embeddedShader.py"
embeddedShaderApp.label = "Embedded Shader"
embeddedShaderApp.iconFile = "app.png"
_drawer.addApp(embeddedShaderApp)

fxdemoApp = AppInfo()
fxdemoApp.name = "./fxdemo.py"
fxdemoApp.label = "Effects Demo"
fxdemoApp.iconFile = "app.png"
_drawer.addApp(fxdemoApp)

loadModelApp = AppInfo()
loadModelApp.name = "./loadModel.py"
loadModelApp.label = "Load Model"
loadModelApp.iconFile = "app.png"
_drawer.addApp(loadModelApp)

_drawer.show()

# vtkApp = AppInfo()
# vtkApp.name = "vtkdemos/Sample"
# vtkApp.label = "Vtk Demo 1"
# vtkApp.iconFile = "vtkdemos/vtkIcon.png"
# _drawer.addApp(vtkApp)

#_scene = getSceneManager()
#_scene.displayWand(0, 0)

# Uncomment to start the specified application by default
#viewer.run("panorama")
