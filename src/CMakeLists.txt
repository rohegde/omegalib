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
	SET(OMEGA_BUILD_APPS false CACHE BOOL "Enable build of applications included in Omegalib")
	SET(OMEGA_BUILD_UTILS false CACHE BOOL "Enable build of utility library ")
	IF(OMEGA_BUILD_UTILS)
		SET(OMEGA_BUILD_DEMOS false CACHE BOOL "Enable build of demo applications")
	ENDIF(OMEGA_BUILD_UTILS)
ENDIF(OMEGA_USE_DISPLAY)

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(3rdparty/libconfig)
ADD_SUBDIRECTORY(omega)

IF(OMEGA_BUILD_UTILS)
	ADD_SUBDIRECTORY(outils)
	ADD_SUBDIRECTORY(oinputserver)
ENDIF(OMEGA_BUILD_UTILS)

IF(OMEGA_BUILD_DEMOS)
	ADD_SUBDIRECTORY(nightfield)
	ADD_SUBDIRECTORY(meshviewer)
	ADD_SUBDIRECTORY(vtkdemo)
ENDIF(OMEGA_BUILD_DEMOS)
