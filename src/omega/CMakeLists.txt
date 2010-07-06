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
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DLIBCONFIGXX_STATIC -DOMEGA_EXPORTING)

#######################################################################################################################
# Exclude libcmt when linking in visual studio
IF(MSVC)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
ENDIF()

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
        ) 
        
#######################################################################################################################
# Enable / disable specific modules within omegalib
SET(OMEGA_USE_MOUSE true CACHE BOOL "Enable/disable mouse input support")
IF(OMEGA_USE_MOUSE)
	SET( srcs	 ${srcs} input/MouseService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/MouseService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_MOUSE)
ENDIF(OMEGA_USE_MOUSE)

SET(OMEGA_USE_MOCAP true CACHE BOOL "Enable/disable NaturalPoint MoCap support")
IF(OMEGA_USE_MOCAP)
	SET( srcs	 ${srcs} input/MoCapService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/MoCapService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_MOCAP)
ENDIF(OMEGA_USE_MOCAP)

SET(OMEGA_USE_TRACKIR true CACHE BOOL "Enable/disable TrackIR support")
IF(OMEGA_USE_TRACKIR)
	SET( srcs	 ${srcs} input/TrackIRService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/TrackIRService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_TRACKIR)
ENDIF(OMEGA_USE_TRACKIR)

SET(OMEGA_USE_PQLABS true CACHE BOOL "Enable/disable PqLabs multitouch support")
IF(OMEGA_USE_PQLABS)
	SET( srcs	 ${srcs} input/PQService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/PQService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_PQLABS)
ENDIF(OMEGA_USE_PQLABS)

SET(OMEGA_USE_NETSERVICE true CACHE BOOL "Enable/disable Network input support")
IF(OMEGA_USE_NETINPUT)
	SET( srcs	 ${srcs} input/NetService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/NetService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_NETSERVICE)
ENDIF(OMEGA_USE_NETSERVICE)

#######################################################################################################################
SOURCE_GROUP(input REGULAR_EXPRESSION input/)		

#######################################################################################################################
# Setup compile info
ADD_LIBRARY( omega SHARED ${srcs} ${headers})

#######################################################################################################################
# Setup module-specific link info
IF(OMEGA_USE_MOCAP)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/NatNetLibStatic optimized ${LIB_RELEASE}/NatNetLibStatic )
ENDIF(OMEGA_USE_MOCAP)

IF(OMEGA_USE_TRACKIR)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/optitrackuuid optimized ${LIB_RELEASE}/optitrackuuid )
ENDIF(OMEGA_USE_TRACKIR)

IF(OMEGA_USE_PQLABS)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/PQMTClient optimized ${LIB_RELEASE}/PQMTClient )
ENDIF(OMEGA_USE_PQLABS)

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( omega
	libconfig
	debug ${LIB_DEBUG}/pthreadVC2 optimized ${LIB_RELEASE}/pthreadVC2
	debug ${LIB_DEBUG}/rpcrt4 optimized ${LIB_RELEASE}/rpcrt4
	debug ${LIB_DEBUG}/equalizer optimized ${LIB_RELEASE}/equalizer
	debug ${LIB_DEBUG}/ws2_32 optimized ${LIB_RELEASE}/ws2_32
	debug ${LIB_DEBUG}/glut32 optimized ${LIB_RELEASE}/glut32
	${OPENGL_LIBRARY})
