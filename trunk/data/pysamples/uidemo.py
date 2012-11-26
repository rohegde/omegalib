from math import *
from euclid import *
from omega import *
from cyclops import *
from omegaToolkit import *

ui = UiModule.createAndInitialize()
wf = ui.getWidgetFactory()
uiroot = ui.getUi()

image = wf.createImage('button', uiroot)
imageFile = loadImage('pysamples/data/btns/Heme-btn.png')
image.setData(imageFile)

size = image.getSize()
size = size / 2;
image.setSize(size)

image.setPosition(Vector2(100, 100))
image.setUIEventCommand('onButtonClick()')

def onButtonClick():
	print('click!')
