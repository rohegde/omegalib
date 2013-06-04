from math import *
from euclid import *
from omega import *
from cyclops import *
from omegaToolkit import *

scene = getSceneManager()

# Create a light
light1 = Light.create()
light1.setColor(Color("#ffffff"))
light1.setAmbient(Color("#444444"))
light1.setEnabled(True)

# Load an animated model
# skelModel = ModelInfo()
# skelModel.name = "skelModel"
# skelModel.path = "examples/data/surgseq/surgseq3.fbx"
# scene.loadModel(skelModel)

# Create a scene object using the loaded animated model, and start the animation
# skel = AnimatedObject.create("skelModel")
# skel.setPosition(Vector3(0, 1.5, -4))
# skel.setScale(Vector3(0.02, 0.02, 0.02))
# skel.setEffect("textured")

ui = UiModule.createAndInitialize()
wf = ui.getWidgetFactory()
uiroot = ui.getUi()

# Create a fully customizable 3d button in 3 steps
# 1: create a ui container with a label, representing the button 
# 2: instead of drawing it on screen, render the container to a texture
# 3: create a box, and apply the texture to it.
container1 = wf.createContainer('container', uiroot, ContainerLayout.LayoutVertical)
container1.setStyle('fill: #00000040; border: 10 #ffffffff;')
container1.setMargin(20)
label1 = wf.createLabel('label', container1, 'Play')
label1.setStyle('font: fonts/arial.ttf 40; color: white')
container1.setPixelOutputEnabled(True)
scene.createTexture('button1Texture', container1.getPixels())

button1 = BoxShape.create(0.3, 0.1, 0.01)
button1.setPosition(Vector3(-1, 2, -3))
button1.setEffect('textured -v emissive -d button1Texture -t')
button1.setSelectable(True)
button1.setName('playButton')
button1.setFacingCamera(getDefaultCamera())

box = BoxShape.create(1,1,1)
box.setEffect('colored -d red')
interactor1 = ToolkitUtils.setupInteractor("config/interactor")
interactor1.setSceneNode(box);

v = False

#--------------------------------------------------------------------------------------------------
def onObjectSelected(node, distance):
	global v
	if(node != None):
		if(node.getName() == 'playButton'): 
			if(v):
				#skel.loopAnimation(0)
				container1.setStyle('fill: black; border: 20 white;')
				label1.setStyle('color: white')
				label1.setText('Stop')
				v = False
			else:
				#skel.stopAllAnimations()
				container1.setStyle('fill: white; border: 20 black;')
				label1.setStyle('color: black')
				label1.setText('Play')
				v = True
			
		
#--------------------------------------------------------------------------------------------------
def onEvent():
	e = getEvent()
	if(e.getServiceType() == ServiceType.Pointer or e.getServiceType() == ServiceType.Wand):
		# Button mappings are different when using wand or mouse
		confirmButton = EventFlags.Button2
		if(e.getServiceType() == ServiceType.Wand): confirmButton = EventFlags.Button5
		
		# When the confirm button is pressed:
		if(e.isButtonDown(confirmButton)):
			r = getRayFromEvent(e)
			if(r[0]): querySceneRay(r[1], r[2], onObjectSelected)

setEventFunction(onEvent)

