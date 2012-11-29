# Example implementation of a slider, using custom images instead of the widget implementation
from math import *
from euclid import *
from omega import *
from cyclops import *
from omegaToolkit import *

# disable camera navigation
getDefaultCamera().setControllerEnabled(False)

# create a background plane
background = PlaneShape.create(4.6, 2.8)
background.setPosition(Vector3(0, 2, -3))
background.setEffect('textured -v emissive -d examples/data/GradientBackground.jpg')

ui = UiModule.createAndInitialize()
wf = ui.getWidgetFactory()
uiroot = ui.getUi()

# slider container
sliderContainer = wf.createContainer('sliderContainer', uiroot, ContainerLayout.LayoutFree)
sliderContainer.setPosition(Vector2(5, 50))

# create the slider background
sliderBackground = wf.createImage('sliderBackground', sliderContainer)
sliderBackground.setData(loadImage('examples/data/btns/slider-text.png'))
sliderBackground.setLayer(WidgetLayer.Back)

# Create the slider element
slider = wf.createImage('button', sliderContainer)
slider.setData(loadImage('examples/data/btns/slider-slide.png'))
slider.setPosition(Vector2(14, 0))
sliderPressed = False
sliderValue = 0

# Create a text item to print the value of the slider
label = wf.createLabel('label', uiroot, 'Slider Value: ' + str(sliderValue))
label.setPosition(Vector2(5, 5))

sliderWidth = 100
sliderContainerWidth = 527

#--------------------------------------------------------------------------------------------------
def onEvent():
	global sliderPressed
	global sliderValue
	global slider
	global sliderContainer
	e = getEvent()
	mousePos = Vector2(e.getPosition().x, e.getPosition().y)
	if(e.getType() == EventType.Move):
		if(sliderPressed == True):
			point = sliderContainer.transformPoint(mousePos)
			if(point.x > sliderWidth and point.x < sliderContainerWidth - sliderWidth / 2):
				newSliderPos = point.x + 14 - sliderWidth
				slider.setPosition(Vector2(newSliderPos, 0))
				sliderValue = int((newSliderPos - 14) * 100 / (sliderContainerWidth - sliderWidth * 3 / 2))
				label.setText('Slider Value: ' + str(sliderValue))
			print(point)
	elif(e.getType() == EventType.Down):
		if(slider.hitTest(mousePos)):
			sliderPressed = True
	elif(e.getType() == EventType.Up):
		sliderPressed = False
	
setEventFunction(onEvent)
