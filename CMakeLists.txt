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
# Setup output Directories 
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
   ${PROJECT_SOURCE_DIR}/bin
   CACHE PATH
   "Single Directory for all Libraries"
   )

#######################################################################################################################
# Setup the Executable output Directory 
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
   ${PROJECT_SOURCE_DIR}/bin
   CACHE PATH
   "Single Directory for all Executables."
   )

#######################################################################################################################
# Setup the Executable output Directory 
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
   ${PROJECT_SOURCE_DIR}/lib
   CACHE PATH
   "Single Directory for all static libraries."
   )
   
#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(src)
