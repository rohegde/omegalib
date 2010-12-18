#********************************************************************************************************************** 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#   Alessandro Febretti							febret@gmail.com
#---------------------------------------------------------------------------------------------------------------------
# [LICENSE NOTE]
#---------------------------------------------------------------------------------------------------------------------
# [SUMMARY OF FILE CONTENTS]
#*********************************************************************************************************************/

#######################################################################################################################
# Directories needed for compilation
INCLUDE_DIRECTORIES(
  ${OmegaLib_SOURCE_DIR}/include/
)

#######################################################################################################################
# Options
SET(OMEGA_USE_DISPLAY false CACHE BOOL "Enable display system support.")
IF(OMEGA_USE_DISPLAY)
	SET(OMEGA_BUILD_UTILITY_LIB false CACHE BOOL "Enable build of the omegalib utility library (outils)")
ENDIF(OMEGA_USE_DISPLAY)
IF(OMEGA_BUILD_UTILITY_LIB)
	SET(OMEGA_BUILD_DEMOS false CACHE BOOL "Enable build of demo applications included in Omegalib")
	SET(OMEGA_BUILD_PLAYGROUND false CACHE BOOL "Enable build of playground projects. Playgroud projects are experimental, and may not work. ")
ENDIF(OMEGA_BUILD_UTILITY_LIB)

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(3rdparty/libconfig)
ADD_SUBDIRECTORY(omega)
ADD_SUBDIRECTORY(oinputserver)

IF(OMEGA_BUILD_UTILITY_LIB)
	ADD_SUBDIRECTORY(outils)
ENDIF(OMEGA_BUILD_UTILITY_LIB)

IF(OMEGA_BUILD_DEMOS)
	ADD_SUBDIRECTORY(demos/meshviewer)
ENDIF(OMEGA_BUILD_DEMOS)

IF(OMEGA_BUILD_PLAYGROUND)
	ADD_SUBDIRECTORY(playground/vtkdemo)
ENDIF(OMEGA_BUILD_PLAYGROUND)
