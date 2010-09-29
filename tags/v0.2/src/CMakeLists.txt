#/********************************************************************************************************************** 
# * THE OMEGA LIB PROJECT
# *---------------------------------------------------------------------------------------------------------------------
# * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# * Authors:										
# *  [Author]									[Mail]
# *---------------------------------------------------------------------------------------------------------------------
# * [LICENSE NOTE]
# *---------------------------------------------------------------------------------------------------------------------
# * [SUMMARY OF FILE CONTENTS]
# *********************************************************************************************************************/

#######################################################################################################################
# Directories needed for compilation
INCLUDE_DIRECTORIES(
  ${OmegaLib_SOURCE_DIR}/include/
)

#######################################################################################################################
# Options
SET(OMEGA_BUILD_OUTK true CACHE BOOL "Enable build of the Omega Utility Toolkit (outk)")
IF(OMEGA_BUILD_OUTK)
	SET(OMEGA_BUILD_APPS true CACHE BOOL "Enable build of applications included in Omegalib")
	IF(OMEGA_BUILD_APPS)
		SET(OMEGA_BUILD_UTILS true CACHE BOOL "Enable build of utility aplications (oinputserver, otest)")
		SET(OMEGA_BUILD_DEMOS true CACHE BOOL "Enable build of demo applications (odemo, nightfield)")
	ENDIF(OMEGA_BUILD_APPS)
ENDIF(OMEGA_BUILD_OUTK)

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(3rdparty/libconfig)
ADD_SUBDIRECTORY(omega)
IF(OMEGA_BUILD_OUTK)
	ADD_SUBDIRECTORY(outk)
ENDIF(OMEGA_BUILD_OUTK)

IF(OMEGA_BUILD_APPS)
	ADD_SUBDIRECTORY(otest)
ENDIF(OMEGA_BUILD_APPS)

IF(OMEGA_BUILD_UTILS)
	ADD_SUBDIRECTORY(oinputserver)
ENDIF(OMEGA_BUILD_UTILS)

IF(OMEGA_BUILD_DEMOS)
	ADD_SUBDIRECTORY(odemo)
	ADD_SUBDIRECTORY(nightfield)
ENDIF(OMEGA_BUILD_DEMOS)
