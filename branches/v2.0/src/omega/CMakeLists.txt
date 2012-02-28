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
add_definitions(-DOMEGA_EXPORTING -DGLEW_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC -DFTGL_LIBRARY_STATIC  -DFREEIMAGE_LIB)

###################################################################################################
# Source files
SET( srcs 
		omega/ApplicationBase.cpp
		omega/Camera.cpp
		omega/Console.cpp
		omega/DrawInterface.cpp
		omega/ServerEngine.cpp
		omega/Font.cpp
		omega/GpuBuffer.cpp
		omega/GpuManager.cpp
		omega/GpuProgram.cpp
		omega/ImageUtils.cpp
		omega/KeyboardService.cpp
		omega/MasterEngine.cpp
		omega/MouseService.cpp
		omega/Node.cpp
        omega/Observer.cpp
		omega/ObserverUpdateService.cpp
        omega/osystem.cpp
		omega/PixelData.cpp
		omega/Pointer.cpp
		omega/PortholeTabletService.cpp
		omega/PythonInterpreter.cpp
		omega/omegaPythonApi.cpp
		omega/Renderer.cpp
		omega/Renderable.cpp
		omega/RenderTarget.cpp
		omega/SagePointerService.cpp
		omega/SceneNode.cpp
		omega/SceneQuery.cpp
		omega/SystemManager.cpp
		omega/Texture.cpp
        )
		
###################################################################################################
# Headers
SET( headers 
		${OmegaLib_SOURCE_DIR}/include/omega.h
		${OmegaLib_SOURCE_DIR}/include/omega/Actor.h
		${OmegaLib_SOURCE_DIR}/include/omega/Application.h
		${OmegaLib_SOURCE_DIR}/include/omega/ApplicationBase.h
		${OmegaLib_SOURCE_DIR}/include/omega/Camera.h
		${OmegaLib_SOURCE_DIR}/include/omega/CLManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/Console.h
		${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
		${OmegaLib_SOURCE_DIR}/include/omega/DrawContext.h
		${OmegaLib_SOURCE_DIR}/include/omega/DrawInterface.h
		${OmegaLib_SOURCE_DIR}/include/omega/ServerEngine.h
		${OmegaLib_SOURCE_DIR}/include/omega/Font.h
		${OmegaLib_SOURCE_DIR}/include/omega/glheaders.h
		${OmegaLib_SOURCE_DIR}/include/omega/GpuBuffer.h
		${OmegaLib_SOURCE_DIR}/include/omega/GpuManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/GpuProgram.h
		${OmegaLib_SOURCE_DIR}/include/omega/ImageUtils.h
		${OmegaLib_SOURCE_DIR}/include/omega/ISceneObject.h
		${OmegaLib_SOURCE_DIR}/include/omega/KeyboardService.h
		${OmegaLib_SOURCE_DIR}/include/omega/MasterEngine.h
		${OmegaLib_SOURCE_DIR}/include/omega/MouseService.h
		${OmegaLib_SOURCE_DIR}/include/omega/Node.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Observer.h
		${OmegaLib_SOURCE_DIR}/include/omega/ObserverUpdateService.h
		${OmegaLib_SOURCE_DIR}/include/omega/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/otypes.h
		${OmegaLib_SOURCE_DIR}/include/omega/PixelData.h
		${OmegaLib_SOURCE_DIR}/include/omega/Pointer.h
		${OmegaLib_SOURCE_DIR}/include/omega/PortholeTabletService.h
		${OmegaLib_SOURCE_DIR}/include/omega/PythonInterpreter.h
		${OmegaLib_SOURCE_DIR}/include/omega/Renderable.h
		${OmegaLib_SOURCE_DIR}/include/omega/RenderPass.h
		${OmegaLib_SOURCE_DIR}/include/omega/RenderTarget.h
		${OmegaLib_SOURCE_DIR}/include/omega/Renderer.h
		${OmegaLib_SOURCE_DIR}/include/omega/SagePointerService.h
		${OmegaLib_SOURCE_DIR}/include/omega/SceneNode.h
		${OmegaLib_SOURCE_DIR}/include/omega/SceneQuery.h
        ${OmegaLib_SOURCE_DIR}/include/omega/SystemManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/Texture.h
		${OmegaLib_SOURCE_DIR}/include/omega/UpdateContext.h
		)
        
###################################################################################################
# Enable / disable specific modules within omegalib

# Equalizer Display Module
set(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
if(OMEGA_USE_DISPLAY_EQUALIZER)
	include(${CMAKE_SOURCE_DIR}/external/UseEqualizer.cmake)
	include_directories(${EQUALIZER_INCLUDES})
	set( srcs ${srcs} 
		omega/EqualizerDisplaySystem.cpp
		omega/eqinternal/ChannelImpl.cpp
		omega/eqinternal/ConfigImpl.cpp
		omega/eqinternal/ViewImpl.cpp
		omega/eqinternal/WindowImpl.cpp
		omega/eqinternal/NodeImpl.cpp
		omega/eqinternal/PipeImpl.cpp
		)
	set( headers ${headers} 
		omega/eqinternal/eqinternal.h
		${OmegaLib_SOURCE_DIR}/include/omega/EqualizerDisplaySystem.h) 
endif(OMEGA_USE_DISPLAY_EQUALIZER)
	
# Glut Display Module
if( WIN32 )
	set(OMEGA_USE_DISPLAY_GLUT true CACHE BOOL "Enable GLUT display system support")
	if(OMEGA_USE_DISPLAY_GLUT)
		set( srcs ${srcs} omega/GlutDisplaySystem.cpp)
		set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/GlutDisplaySystem.h) 
	endif(OMEGA_USE_DISPLAY_GLUT)
	set(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
endif( WIN32 )
	
# OpenCL Module
set(OMEGA_USE_OPENCL false CACHE BOOL "Enable OpenCL support for GPU programs")
if(OMEGA_USE_OPENCL)
	if(NOT WIN32)
		find_package(OpenCL REQUIRED)
	endif(NOT WIN32)
	set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/CLManager.h )
endif(OMEGA_USE_OPENCL)

###################################################################################################
# Group source files into separate folders for visual studio project (this is just eye candy)
source_group(omega REGULAR_EXPRESSION omega/)		
source_group(eqinternal REGULAR_EXPRESSION eqinternal/)		

###################################################################################################
# Create the library with the provided sources and headers
enable_precompiled_headers(omega/precompiled.h srcs)
add_library( omega SHARED ${srcs} ${headers})
target_link_libraries(omega ${OMICRON_LIB} freetype ftgl FreeImage)

###################################################################################################
# Setup module-specific link info
set(EXT_LIB_DIR ${OmegaLib_SOURCE_DIR}/external/libs32)
set(EXT_INCLUDE_DIR ${OmegaLib_SOURCE_DIR}/external/include)

include_directories(${EXT_INCLUDE_DIR})

target_link_libraries( omega ${OPENGL_LIBRARY} glew)

if(OMEGA_USE_DISPLAY_EQUALIZER)
	target_link_libraries( omega ${EQUALIZER_LIBS})
endif(OMEGA_USE_DISPLAY_EQUALIZER)

if(OMEGA_USE_DISPLAY_GLUT)
    if( WIN32 )
	    target_link_libraries(omega freeglut)
	    include_directories(${OmegaLib_SOURCE_DIR}/src/3rdparty/freeglut/include)
    endif( WIN32 )
endif(OMEGA_USE_DISPLAY_GLUT)

if(OMEGA_USE_OPENCL)
	if(WIN32)
		target_link_libraries( omega ${EXT_LIB_DIR}/OpenCL.lib)
	else(WIN32)
		include_directories(${OPENCL_INCLUDE_DIR})
		target_link_libraries( omega ${OPENCL_LIBRARIES})
	endif(WIN32)
endif(OMEGA_USE_OPENCL)

###################################################################################################
# See if we can setup Python support for this build
if(OMEGA_USE_PYTHON)
	target_link_libraries(omega ${PYTHON_LIBRARIES})
endif()

###################################################################################################
configure_file(omega/omegaConfig.h.in ${CMAKE_BINARY_DIR}/include/omegaConfig.h)
