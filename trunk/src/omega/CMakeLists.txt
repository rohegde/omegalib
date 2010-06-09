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
# Find required packages
find_package(GLUT)
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )

#######################################################################################################################
# Source files
SET( srcs 
		Config.cpp
		EqualizerDisplaySystem.cpp
		GLUTDisplaySystem.cpp
		InputManager.cpp
		Log.cpp
		SystemManager.cpp
        Utils.cpp
        gfx/GfxUtils.cpp
		input/MouseService.cpp
		input/TrackIRService.cpp
		input/PQService.cpp
		input/MoCapService.cpp
        )
    
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/Application.h
        ${OmegaLib_SOURCE_DIR}/include/Config.h
        ${OmegaLib_SOURCE_DIR}/include/DisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/EqualizerDisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/GLUTDisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/InputEvent.h
        ${OmegaLib_SOURCE_DIR}/include/InputManager.h
        ${OmegaLib_SOURCE_DIR}/include/Log.h
        ${OmegaLib_SOURCE_DIR}/include/SystemManager.h
        ${OmegaLib_SOURCE_DIR}/include/Utils.h
        ${OmegaLib_SOURCE_DIR}/include/gfx/GfxUtils.h
		${OmegaLib_SOURCE_DIR}/include/input/MouseService.h
		${OmegaLib_SOURCE_DIR}/include/input/TrackIRService.h
		${OmegaLib_SOURCE_DIR}/include/input/PQService.h	
		${OmegaLib_SOURCE_DIR}/include/input/MoCapService.h
        ) 
        
#######################################################################################################################
# Setup compile info
ADD_LIBRARY( omega ${srcs} ${headers})

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( omega
    libconfig
    pthreadVC2
    rpcrt4
    equalizer
    optitrackuuid.lib
    PQMTClient
    NatNetLibStatic
    ws2_32
    ${GLUT_LIBRARY} 
    ${OPENGL_LIBRARY}
)


