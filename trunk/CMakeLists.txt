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
PROJECT(OmegaLib)

if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#######################################################################################################################
# Set binary / lib output dir names
IF(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")
	SET(ARCH "win-x86")
	SET(TOOL "vs9")
	
ELSEIF(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008 Win64")
	SET(ARCH "win-x64")
	SET(TOOL "vs9")
	
ELSEIF(CMAKE_GENERATOR STREQUAL "Visual Studio 10")
	SET(ARCH "win-x86")
	SET(TOOL "vs10")
	
ELSEIF(CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")
	SET(ARCH "win-x64")
	SET(TOOL "vs10")
	
ENDIF(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")

#######################################################################################################################
# Setup output Directories 
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-debug)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-release)

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-debug)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-release)

#######################################################################################################################
# Setup the Executable output Directory 
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/${ARCH}-${TOOL}-debug)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/${ARCH}-${TOOL}-release)

#######################################################################################################################
# Couple utility variables for library linking
SET (LIB_DEBUG ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-debug CACHE STRING "bsd")
SET (LIB_RELEASE ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-release  CACHE STRING "bsd")

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(src)
