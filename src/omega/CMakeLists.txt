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
# take away these and use libs included with project.
find_package(GLUT)
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DLIBCONFIGXX_STATIC)

#######################################################################################################################
# Source files
SET( srcs 
        osystem.cpp
		Config.cpp
		EqualizerDisplaySystem.cpp
		GLUTDisplaySystem.cpp
		InputManager.cpp
		InputService.cpp
        Observer.cpp
		SystemManager.cpp
        Utils.cpp
        gfx/GfxUtils.cpp
        gfx/teapot.cpp
		input/MouseService.cpp
		input/TrackIRService.cpp
		input/PQService.cpp
		input/MoCapService.cpp
        )
    
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/omega/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Application.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Config.h
        ${OmegaLib_SOURCE_DIR}/include/omega/DrawContext.h
        ${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/EqualizerDisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/GLUTDisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputEvent.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputService.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Observer.h
        ${OmegaLib_SOURCE_DIR}/include/omega/SystemManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Utils.h
        ${OmegaLib_SOURCE_DIR}/include/omega/gfx/GfxUtils.h
		${OmegaLib_SOURCE_DIR}/include/omega/input/MouseService.h
		${OmegaLib_SOURCE_DIR}/include/omega/input/TrackIRService.h
		${OmegaLib_SOURCE_DIR}/include/omega/input/PQService.h	
		${OmegaLib_SOURCE_DIR}/include/omega/input/MoCapService.h
        ) 
        
#######################################################################################################################
# Setup compile info
ADD_LIBRARY( omega ${srcs} ${headers})

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( omega
	debug ${LIB_DEBUG}/libconfig optimized ${LIB_RELEASE}/libconfig
	debug ${LIB_DEBUG}/pthreadVC2 optimized ${LIB_RELEASE}/pthreadVC2
	debug ${LIB_DEBUG}/rpcrt4 optimized ${LIB_RELEASE}/rpcrt4
	debug ${LIB_DEBUG}/equalizer optimized ${LIB_RELEASE}/equalizer
	debug ${LIB_DEBUG}/optitrackuuid optimized ${LIB_RELEASE}/optitrackuuid
	debug ${LIB_DEBUG}/PQMTClient optimized ${LIB_RELEASE}/PQMTClient
	debug ${LIB_DEBUG}/NatNetLibStatic optimized ${LIB_RELEASE}/NatNetLibStatic
	debug ${LIB_DEBUG}/ws2_32 optimized ${LIB_RELEASE}/ws2_32
	${GLUT_LIBRARY} 
	${OPENGL_LIBRARY})
