###################################################################################################
# THE OMEGA LIB PROJECT
#-------------------------------------------------------------------------------------------------
# Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  Alessandro Febretti		febret@gmail.com
#-------------------------------------------------------------------------------------------------
# Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
		DisplayConfig.cpp
		Camera.cpp
		CameraController.cpp
		KeyboardMouseCameraController.cpp
		GamepadCameraController.cpp
		MouseCameraController.cpp
		WandCameraController.cpp
		CameraOutput.cpp
		CylindricalDisplayConfig.cpp
		Console.cpp
		DrawInterface.cpp
		EventSharingModule.cpp
		Engine.cpp
		Font.cpp
		GpuBuffer.cpp
		GpuManager.cpp
		GpuProgram.cpp
		ImageUtils.cpp
		KeyboardService.cpp
		ModuleServices.cpp
		MouseService.cpp
		MissionControl.cpp
		Node.cpp
		ObserverUpdateServiceExt.cpp
        osystem.cpp
		PixelData.cpp
		Pointer.cpp
		PythonInterpreter.cpp
		PlanarDisplayConfig.cpp
		omegaPythonApi.cpp
		Renderer.cpp
		Renderable.cpp
		RenderTarget.cpp
		ViewRayService.cpp
		SceneNode.cpp
		SceneQuery.cpp
		SharedDataServices.cpp
		StatsManager.cpp
		SystemManager.cpp
		Texture.cpp
		TextureSource.cpp
		WandEmulationService.cpp
        )
		
###################################################################################################
# Headers
SET( headers 
		${OmegaLib_SOURCE_DIR}/include/omega.h
		${OmegaLib_SOURCE_DIR}/include/omega/Actor.h
		${OmegaLib_SOURCE_DIR}/include/omega/Application.h
		${OmegaLib_SOURCE_DIR}/include/omega/ApplicationBase.h
		${OmegaLib_SOURCE_DIR}/include/omega/AsyncTask.h
		${OmegaLib_SOURCE_DIR}/include/omega/Camera.h
		${OmegaLib_SOURCE_DIR}/include/omega/CameraController.h
		${OmegaLib_SOURCE_DIR}/include/omega/DisplayConfig.h
		${OmegaLib_SOURCE_DIR}/include/omega/KeyboardMouseCameraController.h
		${OmegaLib_SOURCE_DIR}/include/omega/GamepadCameraController.h
		${OmegaLib_SOURCE_DIR}/include/omega/MouseCameraController.h
		${OmegaLib_SOURCE_DIR}/include/omega/WandCameraController.h
		${OmegaLib_SOURCE_DIR}/include/omega/CameraOutput.h
		${OmegaLib_SOURCE_DIR}/include/omega/EventSharingModule.h
		${OmegaLib_SOURCE_DIR}/include/omega/CLManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/Console.h
		${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
		${OmegaLib_SOURCE_DIR}/include/omega/CylindricalDisplayConfig.h
		${OmegaLib_SOURCE_DIR}/include/omega/DrawContext.h
		${OmegaLib_SOURCE_DIR}/include/omega/DrawInterface.h
		${OmegaLib_SOURCE_DIR}/include/omega/Engine.h
		${OmegaLib_SOURCE_DIR}/include/omega/Font.h
		${OmegaLib_SOURCE_DIR}/include/omega/glheaders.h
		${OmegaLib_SOURCE_DIR}/include/omega/GpuBuffer.h
		${OmegaLib_SOURCE_DIR}/include/omega/GpuManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/GpuProgram.h
		${OmegaLib_SOURCE_DIR}/include/omega/ImageUtils.h
		${OmegaLib_SOURCE_DIR}/include/omega/IRendererCommand.h
		${OmegaLib_SOURCE_DIR}/include/omega/ISceneObject.h
		${OmegaLib_SOURCE_DIR}/include/omega/KeyboardService.h
		${OmegaLib_SOURCE_DIR}/include/omega/MissionControl.h
		${OmegaLib_SOURCE_DIR}/include/omega/ModuleServices.h
		${OmegaLib_SOURCE_DIR}/include/omega/MouseService.h
		${OmegaLib_SOURCE_DIR}/include/omega/Node.h
		${OmegaLib_SOURCE_DIR}/include/omega/ObserverUpdateServiceExt.h
		${OmegaLib_SOURCE_DIR}/include/omega/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/otypes.h
		${OmegaLib_SOURCE_DIR}/include/omega/PixelData.h
		${OmegaLib_SOURCE_DIR}/include/omega/PlanarDisplayConfig.h
		${OmegaLib_SOURCE_DIR}/include/omega/Pointer.h
		${OmegaLib_SOURCE_DIR}/include/omega/PythonInterpreter.h
		${OmegaLib_SOURCE_DIR}/include/omega/Renderable.h
		${OmegaLib_SOURCE_DIR}/include/omega/RenderPass.h
		${OmegaLib_SOURCE_DIR}/include/omega/RenderTarget.h
		${OmegaLib_SOURCE_DIR}/include/omega/Renderer.h
		${OmegaLib_SOURCE_DIR}/include/omega/ViewRayService.h
		${OmegaLib_SOURCE_DIR}/include/omega/SceneNode.h
		${OmegaLib_SOURCE_DIR}/include/omega/SceneQuery.h
		${OmegaLib_SOURCE_DIR}/include/omega/SharedDataServices.h
        ${OmegaLib_SOURCE_DIR}/include/omega/SystemManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/StatsManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/Texture.h
		${OmegaLib_SOURCE_DIR}/include/omega/TextureSource.h
		${OmegaLib_SOURCE_DIR}/include/omega/WandEmulationService.h
		)
        
###################################################################################################
# Enable / disable specific modules within omegalib
if(OMEGA_USE_PYTHON)
	add_definitions(-DBOOST_PYTHON_SOURCE)
	set( srcs ${srcs}
			boostPython/dict.cpp
			boostPython/errors.cpp
			boostPython/exec.cpp
			boostPython/import.cpp
			boostPython/list.cpp
			boostPython/long.cpp
			boostPython/module.cpp
			boostPython/numeric.cpp
			boostPython/object_operators.cpp
			boostPython/object_protocol.cpp
			boostPython/slice.cpp
			boostPython/str.cpp
			boostPython/tuple.cpp
			boostPython/wrapper.cpp
			boostPython/converter/arg_to_python_base.cpp
			boostPython/converter/builtin_converters.cpp
			boostPython/converter/from_python.cpp
			boostPython/converter/registry.cpp
			boostPython/converter/type_id.cpp
			boostPython/object/class.cpp
			boostPython/object/enum.cpp
			boostPython/object/function.cpp
			boostPython/object/function_doc_signature.cpp
			boostPython/object/inheritance.cpp
			boostPython/object/iterator.cpp
			boostPython/object/life_support.cpp
			boostPython/object/pickle_support.cpp
			boostPython/object/stl_iterator.cpp
	)
endif()

# Equalizer Display Module
set(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
if(OMEGA_USE_DISPLAY_EQUALIZER)
	include(${CMAKE_SOURCE_DIR}/external/UseEqualizer.cmake)
	include_directories(${EQUALIZER_INCLUDES})
	set( srcs ${srcs} 
		EqualizerDisplaySystem.cpp
		eqinternal/ChannelImpl.cpp
		eqinternal/ConfigImpl.cpp
		eqinternal/WindowImpl.cpp
		eqinternal/NodeImpl.cpp
		eqinternal/PipeImpl.cpp
		)
	set( headers ${headers} 
		eqinternal/eqinternal.h
		${OmegaLib_SOURCE_DIR}/include/omega/EqualizerDisplaySystem.h) 
endif(OMEGA_USE_DISPLAY_EQUALIZER)

# Porthole Module
if(OMEGA_USE_PORTHOLE)
	SET( srcs ${srcs}
			PortholeService.cpp
			PortholeGUI.cpp
			# A tiny JSON library
			vjson/block_allocator.cpp
			vjson/json.cpp
			)
	SET( headers ${headers}
			${OmegaLib_SOURCE_DIR}/include/omega/PortholeService.h
			${OmegaLib_SOURCE_DIR}/include/omega/PortholeGUI.h
			vjson/block_allocator.h
			vjson/json.h
			)
endif()

# SAGE Module
#set(OMEGA_USE_SAGE false CACHE BOOL "Enable SAGE output support")
# if the SAGE_DIRECTORY evn variable exists, enable SAGE support by default
if("$ENV{SAGE_DIRECTORY}" STREQUAL "")
	message("SAGE_DIRECTORY not found: SAGE support DISABLED")
else()
	message("SAGE_DIRECTORY found: SAGE support ENABLED")
	set(SAGE_DIR $ENV{SAGE_DIRECTORY})
	set(SAGE_INCLUDE_DIR ${SAGE_DIR}/include)
	set(OMEGA_USE_SAGE true CACHE BOOL "Enable SAGE support")
	if(WIN32)
		set(SAGE_LIBS 
			${SAGE_DIR}/lib/sail.lib 
			${SAGE_DIR}/win32/lib/pthread.lib 
			${SAGE_DIR}/win32/lib/quanta.lib
			Userenv.lib # needed for GetUserProfileDirectory
			)
	else()
		set(SAGE_LIBS 
			${SAGE_DIR}/lib64/libsail.so
			${SAGE_DIR}/lib64/libquanta.so
			)
	endif()
	
	if(OMEGA_USE_SAGE)
		SET( srcs ${srcs}
				SageManager.cpp
				)
		SET( headers ${headers}
				${OmegaLib_SOURCE_DIR}/include/omega/SageManager.h
				)
	endif()
endif()

# Glut Display Module
if( WIN32 )
	set(OMEGA_USE_DISPLAY_GLUT true CACHE BOOL "Enable GLUT display system support")
	if(OMEGA_USE_DISPLAY_GLUT)
		set( srcs ${srcs} GlutDisplaySystem.cpp)
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
source_group(boostPython REGULAR_EXPRESSION boostPython/)		
source_group(vjson REGULAR_EXPRESSION vjson/)

###################################################################################################
# Create the library with the provided sources and headers
enable_precompiled_headers(precompiled.h srcs)
add_library( omega SHARED ${srcs} ${headers})
target_link_libraries(omega ${OMICRON_LIB} freetype ftgl FreeImage)

###################################################################################################
# Setup module-specific link info
set(EXT_LIB_DIR ${OmegaLib_SOURCE_DIR}/external/libs32)
set(EXT_INCLUDE_DIR ${OmegaLib_SOURCE_DIR}/external/include)

include_directories(${EXT_INCLUDE_DIR})

# Link the right openGL libraries depending on the platform
IF(APPLE)
	target_link_libraries( omega ${OPENGL_LIBRARY})
ELSE(APPLE)
	target_link_libraries( omega ${OPENGL_LIBRARY} glew)
ENDIF(APPLE)

if(OMEGA_USE_DISPLAY_EQUALIZER)
	target_link_libraries( omega ${EQUALIZER_LIBS})
	add_dependencies( omega equalizer )
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
	else()
		include_directories(${OPENCL_INCLUDE_DIR})
		target_link_libraries( omega ${OPENCL_LIBRARIES})
	endif()
endif()

if(OMEGA_USE_SAGE)
	include_directories(${SAGE_INCLUDE_DIR})
	target_link_libraries( omega ${SAGE_LIBS})
endif()

###################################################################################################
# See if we can setup Python support for this build
if(OMEGA_USE_PYTHON)
	if(APPLE)
		add_dependencies( omega Python )
		include_directories(${PYTHON_INCLUDE_DIRS})
	endif(APPLE)
	target_link_libraries(omega ${PYTHON_LIBRARIES})

	# On linux the python thread uses readline to read user input: look for the readline library.
	if(OMEGA_OS_LINUX)
		find_path(READLINE_INCLUDE_DIR readline/readline.h)
		find_library(READLINE_LIBRARY NAMES readline) 

		if(READLINE_INCLUDE_DIR AND READLINE_LIBRARY)
		   set(READLINE_FOUND TRUE)
		endif(READLINE_INCLUDE_DIR AND READLINE_LIBRARY)

		if(READLINE_FOUND)
		   if(NOT Readline_FIND_QUIETLY)
			  message(STATUS "Found GNU readline: ${READLINE_LIBRARY}")
		   endif()
		   add_definitions(-DOMEGA_READLINE_FOUND)
		   target_link_libraries(omega ${READLINE_LIBRARY})
		else(READLINE_FOUND)
		   if(Readline_FIND_REQUIRED)
			  message(FATAL_ERROR "Could not find GNU readline")
		   endif(Readline_FIND_REQUIRED)
		endif(READLINE_FOUND)
	endif()
endif()

###################################################################################################
#
# PORTHOLE
#
if(OMEGA_USE_PORTHOLE)
	# Link against websockets library
	target_link_libraries(omega websockets)
	include_directories(${OmegaLib_SOURCE_DIR}/src/3rdparty/websockets/include)
	if (OMEGA_OS_WIN)
		include_directories(${OmegaLib_SOURCE_DIR}/src/3rdparty/websockets/win32helpers)
		target_link_libraries(omega zlib)
		include_directories(${OmegaLib_SOURCE_DIR}/src/3rdparty/zlib)
	endif()
endif()

# Boost includes
include_directories(${CMAKE_BINARY_DIR}/omicron/omicron/include/boost)

###################################################################################################
configure_file(omegaConfig.h.in ${CMAKE_BINARY_DIR}/include/omegaConfig.h)
