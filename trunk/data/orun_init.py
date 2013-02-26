# This script is executed when orun starts, before running application scripts.
from omega import *
from omegaToolkit import *

mm = MenuManager.createAndInitialize()
mainmnu = mm.createMenu("Main Menu")
mm.setMainMenu(mainmnu)

sysmnu = mainmnu.addSubMenu("System")
sysmnu.addButton("Toggle freefly", ":freefly")
