####################################################################################################################### 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  Alessandro Febretti							febret@gmail.com
#---------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
# following conditions are met:
# 
# Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
# disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
# and the following disclaimer in the documentation and/or other materials provided with the distribution. 
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
# INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
######################################################################################################################

#######################################################################################################################
# Find required packages
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS(-DLIBCONFIGXX_STATIC -DOMEGA_EXPORTING -DGLEW_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC)

#######################################################################################################################
# Source files
SET( srcs 
		omega/glew.c
        omega/osystem.cpp
		omega/Color.cpp
		omega/Config.cpp
		omega/InputManager.cpp
		omega/InputService.cpp
        omega/Observer.cpp
		omega/SystemManager.cpp
		omega/StringUtils.cpp
		omega/RenderTarget.cpp
		omega/DataManager.cpp
		omega/FileDataStream.cpp
		omega/FilesystemDataSource.cpp
        )
		
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/omega/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/otypes.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Color.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Config.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputEvent.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputService.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputFilter.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Observer.h
        ${OmegaLib_SOURCE_DIR}/include/omega/SystemManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/StringUtils.h
        ${OmegaLib_SOURCE_DIR}/include/omega/RenderTarget.h
        ${OmegaLib_SOURCE_DIR}/include/omega/DataManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/FilesystemDataSource.h
        ${OmegaLib_SOURCE_DIR}/include/omega/FileDataStream.h

		# vmml
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/vmmlib.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/vector.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/matrix.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/quaternion.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/math.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/ray.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/sphere.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/plane.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/axis_aligned_box.hpp
        ${OmegaLib_SOURCE_DIR}/include/vmmlib/plane_bounded_volume.hpp
        ) 
        
#######################################################################################################################
# Enable / disable specific modules within omegalib
IF(OMEGA_USE_DISPLAY)
	ADD_DEFINITIONS( -DOMEGA_USE_DISPLAY)
	SET( srcs ${srcs}
			omega/EqualizerDisplaySystem.cpp
			omega/GpuBuffer.cpp
			omega/GpuManager.cpp
			omega/GpuProgram.cpp
			omega/Texture.cpp
			omega/TextureManager.cpp
			)
	SET( headers ${headers}
			${OmegaLib_SOURCE_DIR}/include/omega/Application.h
			${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuBuffer.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuManager.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuProgram.h
			${OmegaLib_SOURCE_DIR}/include/omega/Texture.h
			${OmegaLib_SOURCE_DIR}/include/omega/TextureManager.h	) 
			
	SET(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
	IF(OMEGA_USE_DISPLAY_EQUALIZER)
		ADD_DEFINITIONS( -DOMEGA_USE_DISPLAY_EQUALIZER)
		SET( srcs ${srcs} omega/EqualizerDisplaySystem.cpp)
		SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/EqualizerDisplaySystem.h) 
	ENDIF(OMEGA_USE_DISPLAY_EQUALIZER)
	
	SET(OMEGA_USE_DISPLAY_GLUT true CACHE BOOL "Enable GLUT display system support")
	IF(OMEGA_USE_DISPLAY_GLUT)
		ADD_DEFINITIONS( -DOMEGA_USE_DISPLAY_GLUT)
		SET( srcs ${srcs} omega/GlutDisplaySystem.cpp)
		SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/GlutDisplaySystem.h) 
	ENDIF(OMEGA_USE_DISPLAY_GLUT)
	SET(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
	
	SET(OMEGA_USE_OPENCL true CACHE BOOL "Enable OpenCL support for GPU programs")
	IF(OMEGA_USE_OPENCL)
		ADD_DEFINITIONS( -DOMEGA_USE_OPENCL)
	ENDIF(OMEGA_USE_OPENCL)
ENDIF(OMEGA_USE_DISPLAY)

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
# Group source files into separate folders foe visual studio project (this is just eye candy)
SOURCE_GROUP(omega REGULAR_EXPRESSION omega/)		
SOURCE_GROUP(omega::input REGULAR_EXPRESSION omega/input/)		
SOURCE_GROUP(omega::ui REGULAR_EXPRESSION omega/ui/)		
SOURCE_GROUP(omega::scene REGULAR_EXPRESSION omega/scene/)		
SOURCE_GROUP(vmml REGULAR_EXPRESSION vmmlib)		

#######################################################################################################################
# Setup compile info
IF(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( omega ${srcs} ${headers})
ELSE(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( omega SHARED ${srcs} ${headers})
ENDIF(OMEGA_BUILD_STATIC)

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
		${OPENGL_LIBRARY})
	IF(OMEGA_USE_DISPLAY_EQUALIZER)
		TARGET_LINK_LIBRARIES( omega 
		debug ${LIB_DEBUG}/Equalizer.lib optimized ${LIB_RELEASE}/Equalizer.lib)
	ENDIF(OMEGA_USE_DISPLAY_EQUALIZER)
	IF(OMEGA_USE_DISPLAY_GLUT)
		TARGET_LINK_LIBRARIES( omega 
		debug ${LIB_DEBUG}/freeglut.lib optimized ${LIB_RELEASE}/freeglut.lib)
	ENDIF(OMEGA_USE_DISPLAY_GLUT)
	IF(OMEGA_USE_OPENCL)
		TARGET_LINK_LIBRARIES( omega 
		debug ${LIB_DEBUG}/OpenCL.lib optimized ${LIB_RELEASE}/OpenCL.lib)
	ENDIF(OMEGA_USE_OPENCL)
ENDIF(OMEGA_USE_DISPLAY)

#######################################################################################################################
# Setup link info for additional libraries
TARGET_LINK_LIBRARIES( omega
	libconfig)
