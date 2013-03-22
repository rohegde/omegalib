# This script is executed when orun starts, before running application scripts.
from omega import *

def _onAppStart():
	mm = MenuManager.createAndInitialize()
	mainmnu = mm.createMenu("Main Menu")
	mm.setMainMenu(mainmnu)

	sysmnu = mainmnu.addSubMenu("System")
	mi = sysmnu.addButton("Toggle freefly", ":freefly")

# If config says to start mission control server, do it now.
if(getBoolSetting("config/missionControl", "serverEnabled", False)): mcstart()
