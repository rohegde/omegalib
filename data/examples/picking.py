from math import *
from euclid import *
from omega import *
from cyclops import *
from omegaToolkit import *

ui = UiModule.createAndInitialize()
wf = ui.getWidgetFactory()
uiroot = ui.getUi()

# Create a label
container = wf.createContainer('container', uiroot, ContainerLayout.LayoutFree)
container.setAutosize(False)
container.setSize(Vector2(64, 64))
container.setStyle('fill: #ffff50; border: 10 white;')
label = wf.createLabel('label', container, '1')
label.setPosition(Vector2(-4, 32))
label.setFont("fonts/arial.ttf 20")

# Create a texture from the root ui container contents
container.setPixelOutputEnabled(True)
getSceneManager().createTexture('uiTexture', container.getPixels())

# create a box
box = BoxShape.create(0.5, 0.5, 0.5)
box.setPosition(Vector3(0, 2, -2))
box.setEffect('textured -d uiTexture -t -D -C')

light1 = Light.create()
light1.setColor(Color("#ff5555"))
light1.setAmbient(Color("#aaaaaa"))
light1.setEnabled(True)


