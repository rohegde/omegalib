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

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(3rdparty/libconfig)
ADD_SUBDIRECTORY(omega)
IF(OMEGA_BUILD_OUTK)
ADD_SUBDIRECTORY(outk)
ENDIF(OMEGA_BUILD_OUTK)
ADD_SUBDIRECTORY(otest)
ADD_SUBDIRECTORY(odemo)
ADD_SUBDIRECTORY(oinputserver)
