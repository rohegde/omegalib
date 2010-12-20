#********************************************************************************************************************** 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#   Alessandro Febretti							febret@gmail.com
#---------------------------------------------------------------------------------------------------------------------
# [LICENSE NOTE]
#---------------------------------------------------------------------------------------------------------------------
# Cmake file for omega
#*********************************************************************************************************************/

#######################################################################################################################
# Find required packages
# take away these and use libs included with project.
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS(-DLIBCONFIGXX_STATIC -DOMEGA_EXPORTING)
IF(MSVC)
	# Exclude libcmt when linking in visual studio
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
	ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS)
ENDIF()

#######################################################################################################################
# Source files
SET( srcs 
        omega/osystem.cpp
		omega/Color.cpp
		omega/Config.cpp
		omega/InputManager.cpp
		omega/InputService.cpp
		omega/Math.cpp
        omega/Observer.cpp
		omega/SystemManager.cpp
        omega/Utils.cpp
        )
		
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/omega/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Color.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Config.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputEvent.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputService.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputFilter.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Math.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Observer.h
        ${OmegaLib_SOURCE_DIR}/include/omega/SystemManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Utils.h
        ) 
        
#######################################################################################################################
# Enable / disable specific modules within omegalib
SET(OMEGA_USE_MOUSE true CACHE BOOL "Enable/disable mouse input support")
IF(OMEGA_USE_MOUSE)
	SET( srcs	 ${srcs} omega/input/MouseService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/MouseService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_MOUSE)
ENDIF(OMEGA_USE_MOUSE)

SET(OMEGA_USE_MOCAP true CACHE BOOL "Enable/disable NaturalPoint MoCap support")
IF(OMEGA_USE_MOCAP)
	SET( srcs	 ${srcs} omega/input/MoCapService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/MoCapService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_MOCAP)
ENDIF(OMEGA_USE_MOCAP)

SET(OMEGA_USE_OPTITRACK true CACHE BOOL "Enable/disable OptiTrack camera support")
IF(OMEGA_USE_OPTITRACK)
	SET( srcs	 ${srcs} omega/input/OptiTrackService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/OptiTrackService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_OPTITRACK)
ENDIF(OMEGA_USE_OPTITRACK)

SET(OMEGA_USE_PQLABS true CACHE BOOL "Enable/disable PqLabs multitouch support")
IF(OMEGA_USE_PQLABS)
	SET( srcs	 ${srcs} omega/input/PQService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/PQService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_PQLABS)
ENDIF(OMEGA_USE_PQLABS)

SET(OMEGA_USE_NETSERVICE true CACHE BOOL "Enable/disable Network input support")
IF(OMEGA_USE_NETSERVICE)
	SET( srcs	 ${srcs} omega/input/NetService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/NetService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_NETSERVICE)
ENDIF(OMEGA_USE_NETSERVICE)

#######################################################################################################################
# Add code and library dependencies when the equalizer display system is enabled
IF(OMEGA_USE_DISPLAY)
	ADD_DEFINITIONS( -DOMEGA_USE_DISPLAY)
	SET( srcs 
			${srcs}
			omega/DisplaySystem.cpp
			omega/GfxUtils.cpp
			omega/GpuBuffer.cpp
			omega/GpuManager.cpp
			omega/GpuProgram.cpp
			omega/teapot.cpp
			)
	SET( headers 
			${OmegaLib_SOURCE_DIR}/include/omega/Application.h
			${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
			${OmegaLib_SOURCE_DIR}/include/omega/GfxUtils.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuBuffer.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuManager.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuProgram.h
			) 
ENDIF(OMEGA_USE_DISPLAY)

#######################################################################################################################
# Group source files into separate folders foe visual studio project (this is just eye candy)
SOURCE_GROUP(omega REGULAR_EXPRESSION omega/)		
SOURCE_GROUP(omega::input REGULAR_EXPRESSION omega/input/)		
SOURCE_GROUP(omega::ui REGULAR_EXPRESSION omega/ui/)		
SOURCE_GROUP(omega::scene REGULAR_EXPRESSION omega/scene/)		

#######################################################################################################################
# Setup compile info
ADD_LIBRARY( omega ${srcs} ${headers})

#######################################################################################################################
# Setup module-specific link info
IF(OMEGA_USE_MOCAP)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/NatNetLibStatic.lib optimized ${LIB_RELEASE}/NatNetLibStatic.lib)
ENDIF(OMEGA_USE_MOCAP)

IF(OMEGA_USE_OPTITRACK)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/optitrackuuid.lib optimized ${LIB_RELEASE}/optitrackuuid.lib)
ENDIF(OMEGA_USE_OPTITRACK)

IF(OMEGA_USE_PQLABS)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/PQMTClient.lib optimized ${LIB_RELEASE}/PQMTClient.lib)
ENDIF(OMEGA_USE_PQLABS)

IF(OMEGA_USE_PQLABS)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/PQMTClient.lib optimized ${LIB_RELEASE}/PQMTClient.lib)
ENDIF(OMEGA_USE_PQLABS)

IF(OMEGA_USE_NETSERVICE)
	IF(WIN32)
		TARGET_LINK_LIBRARIES( omega ws2_32.lib)
	ENDIF(WIN32)
ENDIF(OMEGA_USE_NETSERVICE)

IF(OMEGA_USE_DISPLAY)
	TARGET_LINK_LIBRARIES( omega
		debug ${LIB_DEBUG}/pthreadVC2.lib optimized ${LIB_RELEASE}/pthreadVC2.lib
		debug ${LIB_DEBUG}/equalizer.lib optimized ${LIB_RELEASE}/equalizer.lib
		debug ${LIB_DEBUG}/glut32.lib optimized ${LIB_RELEASE}/glut32.lib
		debug ${LIB_DEBUG}/OpenCL.lib optimized ${LIB_RELEASE}/OpenCL.lib
		${OPENGL_LIBRARY})
ENDIF(OMEGA_USE_DISPLAY)

#######################################################################################################################
# Setup link info for additional libraries
TARGET_LINK_LIBRARIES( omega
	libconfig)
