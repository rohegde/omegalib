# This script is executed when orun starts, before running application scripts.
from omega import *
from cyclops import *

speedLabel = None

def _setCamSpeed(speedLevel):
	global speedLabel
	s = 10 ** (speedLevel - 4)
	speedLabel.setText("Navigation Speed: " + str(s) + "x")
	cc = getDefaultCamera().getController()
	if(cc != None):
		cc.setSpeed(s)

def _resetCamera():
	getDefaultCamera().setPosition(Vector3(0, 0, 0))
	getDefaultCamera().setPitchYawRoll(Vector3(0, 0, 0))
		
def _autonearfar(value):
	if(value):
		queueCommand(":autonearfar on")
	else:
		queueCommand(":autonearfar off")

def _displayWand(value):
	if(value):
		getSceneManager().displayWand(0, 1)
	else:
		getSceneManager().hideWand(0)
		
	
def _onAppStart():
    # mm = getViewer().getMenuManager()
	mm = MenuManager.createAndInitialize()
	mainmnu = mm.createMenu("Main Menu")
	
	mi = mainmnu.addImage(loadImage("omegalib-transparent-white.png"))
	ics = mi.getImage().getSize() * 0.1
	mi.getImage().setSize(ics)
	
	mm.setMainMenu(mainmnu)
	sysmnu = mainmnu.addSubMenu("System")
	mi = sysmnu.addButton("Toggle freefly", ":freefly")
	mi = sysmnu.addButton("Reset", "_resetCamera()")
	mi = sysmnu.addButton("Auto Near / Far", "_autonearfar(%value%)")
	mi.getButton().setCheckable(True)
	mi = sysmnu.addButton("Display Wand", "_displayWand(%value%)")
	mi.getButton().setCheckable(True)
	global speedLabel
	speedLabel = sysmnu.addLabel("sd")
	_setCamSpeed(4)
	ss = sysmnu.addSlider(10, "_setCamSpeed(%value%)")
	ss.getSlider().setValue(4)
	ss.getWidget().setWidth(200)
	
	mi = sysmnu.addButton("Toggle Stereo", "toggleStereo()")
	mi = sysmnu.addButton("Toggle Console", ":c")
	mi = sysmnu.addButton("List Active Modules", "printModules()")
	mi = sysmnu.addButton("Exit omegalib", "oexit()")
	

# If config says to start mission control server, do it now.
if(getBoolSetting("config/missionControl", "serverEnabled", False)): mcstart()
