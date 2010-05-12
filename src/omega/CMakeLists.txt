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
  ${OmegaLib_SOURCE_DIR}/include/
)

LINK_DIRECTORIES(
  ${OmegaLib_BINARY_DIR}/3rdparty/libconfig
)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )

#######################################################################################################################
# Source files
SET( srcs 
        olist.c
        odisplay.c
        oevent.c
        oinput.c
        omega.c
        ostate.c
        othread.c
        outil.c
        olog.c
        display/oglsimple.c
        input/omouse.c
        )
    
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/olist.h
        ${OmegaLib_SOURCE_DIR}/include/odisplay.h
        ${OmegaLib_SOURCE_DIR}/include/oevent.h
        ${OmegaLib_SOURCE_DIR}/include/oinput.h
        ${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/ostate.h
        ${OmegaLib_SOURCE_DIR}/include/othread.h
        ${OmegaLib_SOURCE_DIR}/include/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/outil.h
        ${OmegaLib_SOURCE_DIR}/include/olog.h
        display/oglsimple.h
        input/omouse.h
        ) 
        
#######################################################################################################################
# Setup compile info
ADD_LIBRARY( omega ${srcs} ${headers})

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( omega
    libconfig
)


