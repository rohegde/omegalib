# This script is executed when orun starts, before running application scripts.
from omega import *

speedLabel = None

def _setCamSpeed(speedLevel):
	global speedLabel
	s = 10 ** (speedLevel - 4)
	speedLabel.setText("Navigation Speed: " + str(s) + "x")
	cc = getDefaultCamera().getController()
	if(cc != None):
		cc.setSpeed(s)

def _onAppStart():
	mm = MenuManager.createAndInitialize()
	mainmnu = mm.createMenu("Main Menu")
	mm.setMainMenu(mainmnu)

	sysmnu = mainmnu.addSubMenu("System")
	mi = sysmnu.addButton("Toggle freefly", ":freefly")

	global speedLabel
	speedLabel = sysmnu.addLabel("sd")
	_setCamSpeed(1)
	ss = sysmnu.addSlider("speed", "_setCamSpeed(%value%)")
	ss.getSlider().setTicks(10)
	ss.getSlider().setValue(5)
	ss.getWidget().setWidth(200)

# If config says to start mission control server, do it now.
if(getBoolSetting("config/missionControl", "serverEnabled", False)): mcstart()
