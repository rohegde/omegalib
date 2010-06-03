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
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/libconfig
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/equalizer/include
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/gl/include
  ${OmegaLib_SOURCE_DIR}/include/
)

LINK_DIRECTORIES(
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/equalizer/lib
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/gl/lib
  ${OmegaLib_BINARY_DIR}/3rdparty/libconfig
)

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(3rdparty/libconfig)
ADD_SUBDIRECTORY(omega)
ADD_SUBDIRECTORY(otest)
