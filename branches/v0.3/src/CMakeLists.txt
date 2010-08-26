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
SET(OMEGA_BUILD_APPS true CACHE BOOL "Enable build of applications included in Omegalib")
SET(OMEGA_BUILD_UTILS true CACHE BOOL "Enable build of utility library ")
IF(OMEGA_BUILD_APPS)
	SET(OMEGA_BUILD_DEMOS true CACHE BOOL "Enable build of demo applications (nightfield, playground)")
ENDIF(OMEGA_BUILD_APPS)

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(3rdparty/libconfig)
ADD_SUBDIRECTORY(omega)
IF(OMEGA_BUILD_UTILS)
	ADD_SUBDIRECTORY(outils)
ENDIF(OMEGA_BUILD_UTILS)

IF(OMEGA_BUILD_DEMOS)
	ADD_SUBDIRECTORY(nightfield)
	ADD_SUBDIRECTORY(playground)
ENDIF(OMEGA_BUILD_DEMOS)
