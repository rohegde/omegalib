###################################################################################################
# THE OMEGA LIB PROJECT
#-------------------------------------------------------------------------------------------------
# Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  Alessandro Febretti		febret@gmail.com
#-------------------------------------------------------------------------------------------------
# Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification, are permitted 
# provided that the following conditions are met:
# 
# Redistributions of source code must retain the above copyright notice, this list of conditions 
# and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
# notice, this list of conditions and the following disclaimer in the documentation and/or other 
# materials provided with the distribution. 
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
###################################################################################################

###################################################################################################
# Find required packages
find_package(OpenGL)

###################################################################################################
# Compile definitions
add_definitions(-DOMEGA_EXPORTING -DGLEW_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC)

###################################################################################################
# Source files
SET( srcs 
        omega/osystem.cpp
		omega/Color.cpp
		omega/Config.cpp
		omega/InputManager.cpp
		omega/InputService.cpp
        omega/Observer.cpp
		omega/SystemManager.cpp
		omega/StringUtils.cpp
		omega/DataManager.cpp
		omega/FileDataStream.cpp
		omega/FilesystemDataSource.cpp
        )
		
###################################################################################################
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
        
###################################################################################################
# Enable / disable specific modules within omegalib
if(OMEGA_USE_DISPLAY)
	add_definitions( -DOMEGA_USE_DISPLAY)
	set( srcs ${srcs}
			omega/EqualizerDisplaySystem.cpp
			omega/GpuBuffer.cpp
			omega/GpuManager.cpp
			omega/GpuProgram.cpp
			omega/RenderTarget.cpp
			omega/Texture.cpp
			omega/TextureManager.cpp
			)
	set( headers ${headers}
			${OmegaLib_SOURCE_DIR}/include/omega/Application.h
			${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuBuffer.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuManager.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuProgram.h
			${OmegaLib_SOURCE_DIR}/include/omega/RenderTarget.h
			${OmegaLib_SOURCE_DIR}/include/omega/Texture.h
			${OmegaLib_SOURCE_DIR}/include/omega/TextureManager.h	) 
	
	# Equalizer Display Module
	set(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
	if(OMEGA_USE_DISPLAY_EQUALIZER)
		add_definitions( -DOMEGA_USE_DISPLAY_EQUALIZER)
		set( srcs ${srcs} omega/EqualizerDisplaySystem.cpp)
		set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/EqualizerDisplaySystem.h) 
	endif(OMEGA_USE_DISPLAY_EQUALIZER)
	
	# Glut Display Module
	set(OMEGA_USE_DISPLAY_GLUT true CACHE BOOL "Enable GLUT display system support")
	if(OMEGA_USE_DISPLAY_GLUT)
		add_definitions( -DOMEGA_USE_DISPLAY_GLUT)
		set( srcs ${srcs} omega/GlutDisplaySystem.cpp)
		set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/GlutDisplaySystem.h) 
	endif(OMEGA_USE_DISPLAY_GLUT)
	SET(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
	
	# OpenCL Module
	set(OMEGA_USE_OPENCL false CACHE BOOL "Enable OpenCL support for GPU programs")
	if(OMEGA_USE_OPENCL)
		add_definitions( -DOMEGA_USE_OPENCL)
	endif(OMEGA_USE_OPENCL)
endif(OMEGA_USE_DISPLAY)

set(OMEGA_USE_MOUSE true CACHE BOOL "Enable/disable mouse input support")
if(OMEGA_USE_MOUSE)
	set( srcs	 ${srcs} omega/input/MouseService.cpp )
	set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/MouseService.h )
	add_definitions( -DOMEGA_USE_MOUSE)
endif(OMEGA_USE_MOUSE)

if(WIN32)
	set(OMEGA_USE_NATURAL_POINT true CACHE BOOL "Enable/disable NaturalPoint MoCap support")
	if(OMEGA_USE_NATURAL_POINT)
		set( srcs	 ${srcs} omega/input/NaturalPointService.cpp )
		set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/NaturalPointService.h )
		add_definitions( -DOMEGA_USE_NATURAL_POINT)
	ENDIF(OMEGA_USE_NATURAL_POINT)

	set(OMEGA_USE_OPTITRACK true CACHE BOOL "Enable/disable OptiTrack camera support")
	if(OMEGA_USE_OPTITRACK)
		set( srcs	 ${srcs} omega/input/OptiTrackService.cpp )
		SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/OptiTrackService.h )
		add_definitions( -DOMEGA_USE_OPTITRACK)
	endif(OMEGA_USE_OPTITRACK)

	set(OMEGA_USE_PQLABS true CACHE BOOL "Enable/disable PqLabs multitouch support")
	if(OMEGA_USE_PQLABS)
		set( srcs	 ${srcs} omega/input/PQService.cpp )
		set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/PQService.h )
		add_definitions( -DOMEGA_USE_PQLABS)
	endif(OMEGA_USE_PQLABS)

	set(OMEGA_USE_NETSERVICE true CACHE BOOL "Enable/disable Network input support")
	if(OMEGA_USE_NETSERVICE)
		set( srcs	 ${srcs} omega/input/NetService.cpp )
		set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/NetService.h )
		add_definitions( -DOMEGA_USE_NETSERVICE)
	endif(OMEGA_USE_NETSERVICE)

	SET(OMEGA_USE_OPENNI true CACHE BOOL "Enable/disable OpenNI support")
	IF(OMEGA_USE_OPENNI)
		SET( srcs	 ${srcs} omega/input/OpenNIService.cpp )
		SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/OpenNIService.h )
		ADD_DEFINITIONS( -DOMEGA_USE_OPENNI)
		INCLUDE_DIRECTORIES(${OmegaLib_SOURCE_DIR}/ext/windows/include/openni)
ENDIF(OMEGA_USE_OPENNI)
endif(WIN32)
###################################################################################################
# Group source files into separate folders foe visual studio project (this is just eye candy)
source_group(omega REGULAR_EXPRESSION omega/)		
source_group(omega::input REGULAR_EXPRESSION omega/input/)		
source_group(omega::ui REGULAR_EXPRESSION omega/ui/)		
source_group(omega::scene REGULAR_EXPRESSION omega/scene/)		
source_group(vmml REGULAR_EXPRESSION vmmlib)		

###################################################################################################
# Create the library with the provided sources and headers
if(OMEGA_BUILD_STATIC)
	add_library( omega ${srcs} ${headers})
else(OMEGA_BUILD_STATIC)
	add_library( omega SHARED ${srcs} ${headers})
endif(OMEGA_BUILD_STATIC)

###################################################################################################
# Setup module-specific link info
set(EXT_LIB_DIR ${OmegaLib_SOURCE_DIR}/ext/windows/libs32)
set(EXT_INCLUDE_DIR ${OmegaLib_SOURCE_DIR}/ext/windows/include)

include_directories(${EXT_INCLUDE_DIR})

if(OMEGA_USE_NATURAL_POINT)
	if(WIN32)
		target_link_libraries( omega ${EXT_LIB_DIR}/NatNetLibStatic.lib)
	endif(WIN32)
endif(OMEGA_USE_NATURAL_POINT)

if(OMEGA_USE_OPTITRACK)
	if(WIN32)
		target_link_libraries( omega ${EXT_LIB_DIR}/optitrackuuid.lib)
	endif(WIN32)
endif(OMEGA_USE_OPTITRACK)

if(OMEGA_USE_PQLABS)
	if(WIN32)
		target_link_libraries( omega ${EXT_LIB_DIR}/PQMTClient.lib)
		# Copy the PQLabs dll into the destination bin directory
		configure_file(${EXT_LIB_DIR}/PQMTClient.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG} COPYONLY)
		configure_file(${EXT_LIB_DIR}/PQMTClient.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} COPYONLY)
	endif(WIN32)
endif(OMEGA_USE_PQLABS)

if(OMEGA_USE_NETSERVICE)
	if(WIN32)
		target_link_libraries( omega ws2_32.lib)
	endif(WIN32)
endif(OMEGA_USE_NETSERVICE)

IF(OMEGA_USE_OPENNI)
	IF(WIN32)
		TARGET_LINK_LIBRARIES( omega ${EXT_LIB_DIR}/openNI.lib )
	ENDIF(WIN32)
ENDIF(OMEGA_USE_OPENNI)

if(OMEGA_USE_DISPLAY_EQUALIZER)
	target_link_libraries(omega Equalizer)
endif(OMEGA_USE_DISPLAY_EQUALIZER)

if(OMEGA_USE_DISPLAY_GLUT)
	target_link_libraries(omega freeglut)
	include_directories(${OmegaLib_SOURCE_DIR}/src/3rdparty/freeglut/include)
endif(OMEGA_USE_DISPLAY_GLUT)

if(OMEGA_USE_OPENCL)
	if(WIN32)
		target_link_libraries( omega ${EXT_LIB_DIR}/OpenCL.lib)
	endif(WIN32)
endif(OMEGA_USE_OPENCL)
###################################################################################################
# Setup link info for additional libraries
target_link_libraries( omega ${OPENGL_LIBRARY} glew libconfig Collage)
