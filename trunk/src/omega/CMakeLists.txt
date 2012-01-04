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
if(OMEGA_USE_DISPLAY)
	find_package(OpenGL)
endif(OMEGA_USE_DISPLAY)

###################################################################################################
# Compile definitions
add_definitions(-DOMEGA_EXPORTING -DGLEW_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC)

###################################################################################################
# Source files
SET( srcs 
        omega/osystem.cpp
        omega/Observer.cpp
		omega/SystemManager.cpp
        )
		
###################################################################################################
# Headers
SET( headers 
		${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/omega/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/otypes.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Observer.h
        ${OmegaLib_SOURCE_DIR}/include/omega/SystemManager.h
        ) 
        
###################################################################################################
# Enable / disable specific modules within omegalib
if(OMEGA_USE_PYTHON)
	set( headers ${headers}
		omega/PythonInterpreterWrapper.h
		${OmegaLib_SOURCE_DIR}/include/omega/PythonInterpreter.h)

	set( srcs ${srcs}
		omega/PythonInterpreter.cpp)
endif(OMEGA_USE_PYTHON)

if(OMEGA_USE_DISPLAY)
	set( srcs ${srcs}
			omega/Application.cpp
			omega/GpuBuffer.cpp
			omega/GpuManager.cpp
			omega/GpuProgram.cpp
			omega/RenderTarget.cpp
			omega/SagePointerService.cpp
			omega/PixelData.cpp
			omega/Texture.cpp
 		    omega/ObserverUpdateService.cpp
			)
	set( headers ${headers}
			${OmegaLib_SOURCE_DIR}/include/omega/Application.h
			${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuBuffer.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuManager.h
			${OmegaLib_SOURCE_DIR}/include/omega/GpuProgram.h
			${OmegaLib_SOURCE_DIR}/include/omega/RenderTarget.h
			${OmegaLib_SOURCE_DIR}/include/omega/SagePointerService.h
			${OmegaLib_SOURCE_DIR}/include/omega/PixelData.h
			${OmegaLib_SOURCE_DIR}/include/omega/Texture.h
	        ${OmegaLib_SOURCE_DIR}/include/omega/ObserverUpdateService.h)
	
	# Equalizer Display Module
	if(OMEGA_USE_DISPLAY_EQUALIZER)
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

	# Mouse input support	
		set(OMEGA_USE_MOUSE true CACHE BOOL "Enable/disable mouse input support")
		if(OMEGA_USE_MOUSE)
			set( srcs	 ${srcs} omega/MouseService.cpp )
			set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/MouseService.h )
		endif(OMEGA_USE_MOUSE)
		
	# Keyboard input support	
	set(OMEGA_USE_KEYBOARD true CACHE BOOL "Enable/disable mouse input support")
	if(OMEGA_USE_KEYBOARD)
		set( srcs	 ${srcs} omega/KeyboardService.cpp )
		set( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/KeyboardService.h )
	endif(OMEGA_USE_KEYBOARD)
endif(OMEGA_USE_DISPLAY)

###################################################################################################
# Group source files into separate folders for visual studio project (this is just eye candy)
source_group(omega REGULAR_EXPRESSION omega/)		

###################################################################################################
# Create the library with the provided sources and headers
enable_precompiled_headers(omega/precompiled.h srcs)
add_library( omega SHARED ${srcs} ${headers})
target_link_libraries(omega ${OMICRON_LIB})

###################################################################################################
# Setup module-specific link info
set(EXT_LIB_DIR ${OmegaLib_SOURCE_DIR}/external/libs32)
set(EXT_INCLUDE_DIR ${OmegaLib_SOURCE_DIR}/external/include)

include_directories(${EXT_INCLUDE_DIR})

if(OMEGA_USE_DISPLAY)
	target_link_libraries( omega ${OPENGL_LIBRARY} glew)
endif(OMEGA_USE_DISPLAY)

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

if(OMEGA_USE_PYTHON)
	target_link_libraries(omega ${PYTHON_LIBRARIES})
endif(OMEGA_USE_PYTHON)

###################################################################################################
configure_file(omega/osysdefs.h.in ${CMAKE_BINARY_DIR}/include/osysdefs.h)
