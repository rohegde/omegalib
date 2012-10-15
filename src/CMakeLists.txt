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
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
include(ExternalProject)
#include(OmegaAddTutorial)
include(UsePch)

include(${OmegaLib_SOURCE_DIR}/external/UseOmicron.cmake)

###################################################################################################
# Visual Leak Detector Support
if(OMEGA_OS_WIN)
	set(OMEGA_ENABLE_VLD false CACHE BOOL "Enables Visual Leak Detector support for this build.")
endif()

###################################################################################################
# See if we can setup Python support for this build
set(OMEGA_ENABLE_PYTHON true CACHE BOOL "Enables python support if available.")
if(OMEGA_ENABLE_PYTHON)
	find_package(PythonLibs)
	if(PYTHONLIBS_FOUND)
		set(OMEGA_USE_PYTHON true)
		include_directories(${PYTHON_INCLUDE_DIRS})
	else()
		message("Python libraries not found on this system. Omegalib Python support will be disabled")
	endif()
else()
	set(OMEGA_USE_PYTHON false)
endif()

# Enable / disable porthole support
set(OMEGA_USE_PORTHOLE true CACHE BOOL "Enable Porthole support")

###################################################################################################
# Options
set(OMEGA_BUILD_OSG_LIB false CACHE BOOL "Enable build of OpenSceneGraph integration library")
set(OMEGA_BUILD_VTK_LIB false CACHE BOOL "Enable build of vtk integration library (Vtk has to be compiled with Python support)")

###################################################################################################
# Set include paths
include_directories(
  ${CMAKE_BINARY_DIR}/include/
  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/src/3rdparty/glew  
  ${CMAKE_SOURCE_DIR}/external/include
  ${OMICRON_INCLUDE_DIRS}
  )

###################################################################################################
# Add libraries and applications to the project.

# Add some libraries used for display support.
include(${OmegaLib_SOURCE_DIR}/external/UseFreeImage.cmake)

include(${OmegaLib_SOURCE_DIR}/external/UseFreeType.cmake)
include(${OmegaLib_SOURCE_DIR}/external/UseFtgl.cmake)
include_directories(
  ${OmegaLib_BINARY_DIR}/ftgl/src/
  ${OmegaLib_BINARY_DIR}/freetype/include/
  ${OmegaLib_BINARY_DIR}/FreeImage/Source/
)

if(OMEGA_USE_PORTHOLE)
	# PORTHOLE Add websockets lib and zlib if on windows
	if(OMEGA_OS_WIN)
		add_subdirectory(3rdparty/zlib)
	endif(OMEGA_OS_WIN)
	add_subdirectory(3rdparty/websockets)
	add_subdirectory(apps/porthello)
endif()

add_subdirectory(3rdparty/glew)
add_subdirectory(apps/ohello)
add_subdirectory(apps/ohello2)
add_subdirectory(apps/omegaSound)
add_subdirectory(omegaToolkit)

# OSX uses its own glut, do not compile it internally (Should we do the same under linux?)
if(OMEGA_USE_DISPLAY_GLUT) 
    if(NOT APPLE) 
	    add_subdirectory(3rdparty/freeglut)
    endif()
endif()

# Open Scene Graph configuration, libraries and applications
if(OMEGA_BUILD_OSG_LIB)
	include(${OmegaLib_SOURCE_DIR}/external/UseOpenSceneGraph.cmake)
	include_directories(${OSG_INCLUDES})
	add_subdirectory(omegaOsg)
	add_subdirectory(cyclops)
	add_subdirectory(apps/cyhello)
	add_subdirectory(apps/cyhello2)
	add_subdirectory(apps/osgviewer)
	if(OMEGA_USE_PORTHOLE)
		add_subdirectory(apps/portholeCustomDraw)
	endif()
endif()

# vtk configuration, libraries and applications
if(OMEGA_BUILD_VTK_LIB)
	find_package(VTK)
	if(NOT VTK_DIR)
	  message(FATAL_ERROR "Please set VTK_DIR.")
	endif(NOT VTK_DIR)
	include(${VTK_USE_FILE})
	
	add_subdirectory(omegaVtk)
	add_subdirectory(apps/vtkhello)
endif(OMEGA_BUILD_VTK_LIB)

# Sets the path to the omega applications source root (if present)
set(OMEGA_APP_DIR CACHE PATH "omegalib application directory")
if(OMEGA_APP_DIR)
	add_subdirectory(${OMEGA_APP_DIR} ${CMAKE_BINARY_DIR}/apps)
	file(TO_CMAKE_PATH ${OMEGA_APP_DIR} OMEGA_FIXED_APP_DIR)
	# Set the OMEGA_APPROOT_DIRECTORY to the app directory root
	# (it will be used to set a data lookup path)
	add_definitions(-DOMEGA_APPROOT_DIRECTORY="${OMEGA_FIXED_APP_DIR}")
endif()

# Add the core omega project
add_subdirectory(omega)

if(PYTHONLIBS_FOUND)
	# We need to add orun here because its config file uses build options defined above
	add_subdirectory(apps/orun)
	add_subdirectory(apps/mcsend)
endif()

configure_file(${CMAKE_SOURCE_DIR}/CMakeModules/UseOmegalib.cmake.in ${CMAKE_BINARY_DIR}/UseOmegalib.cmake)
if(WIN32)
	configure_file(${CMAKE_SOURCE_DIR}/MakeDistro.bat.in ${CMAKE_BINARY_DIR}/MakeDistro.bat)
endif()


