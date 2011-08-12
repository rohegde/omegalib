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

# TODO: move this to omega CMakeFile, inside Equalizer block
# Check if 32bit MAC or 64bit MAC
if( APPLE )
    if(_CMAKE_OSX_MACHINE MATCHES "64")
        option(EQUALIZER_PREFER_AGL "Prefer AGL over GLX on Mac OS X" OFF)
    endif()
endif( APPLE )

###################################################################################################
# Options
set(OMEGA_BUILD_STATIC false CACHE BOOL "Build omega as a static library")
if(OMEGA_BUILD_STATIC)
	add_definitions(-DOMEGA_STATIC)
endif(OMEGA_BUILD_STATIC)

set(OMEGA_BUILD_APPS false CACHE BOOL "Enable building of omegalib applications. When disabled, only libraries will be built.")

set(OMEGA_USE_DISPLAY false CACHE BOOL "Enable display system support.")
if(OMEGA_USE_DISPLAY)
	set(OMEGA_BUILD_VTK_LIB false CACHE BOOL "Enable build of vtk integration library (Vtk has to be compiled with Python support)")
	set(OMEGA_BUILD_OSG_LIB false CACHE BOOL "Enable build of OpenSceneGraph integration library")
	set(OMEGA_BUILD_QT_LIB false CACHE BOOL "Enable build of Qt integration library")
endif(OMEGA_USE_DISPLAY)

set(OMEGA_USE_PYTHON false CACHE BOOL "Enable python scripting support.")
if(OMEGA_USE_PYTHON)
	find_package(PythonLibs)
	if(PYTHONLIBS_FOUND)
		include_directories(${PYTHON_INCLUDE_DIRS})
	else(PYTHONLIBS_FOUND)
		message(ERROR "Python libraries not found")
	endif(PYTHONLIBS_FOUND)
	add_definitions(-DOMEGA_USE_PYTHON)
endif(OMEGA_USE_PYTHON)

###################################################################################################
# Set include paths
include_directories(
  ${OmegaLib_SOURCE_DIR}/include/
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/boost
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/libconfig  
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/glew  
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/ftgl/src/
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/freetype/include/
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/FreeImage/Source/
  ${OmegaLib_BINARY_DIR}/src/3rdparty/equalizer/include/
  )

###################################################################################################
# Add libraries and applications to the project.

add_subdirectory(3rdparty/libconfig)
add_subdirectory(3rdparty/equalizer)

# TODO: should this be moved inside then omega OPENNI module block?
if(WIN32)
	include_directories($ENV{PROGRAMFILES}/OpenNI/Include)
endif(WIN32)

# Add some libraries used for display support.
if(OMEGA_USE_DISPLAY)
	add_subdirectory(3rdparty/FreeImage)
	add_subdirectory(3rdparty/freetype)
	add_subdirectory(3rdparty/ftgl)
	add_subdirectory(3rdparty/glew)
endif(OMEGA_USE_DISPLAY)

# OSX uses its own glut, do not compile it internally (Should we do the same under linux?)
if(OMEGA_USE_DISPLAY_GLUT) 
    if(NOT APPLE) 
	    add_subdirectory(3rdparty/freeglut)
    endif(NOT APPLE)
endif(OMEGA_USE_DISPLAY_GLUT)

# Add the core omega project
add_subdirectory(omega)

if(OMEGA_BUILD_APPS)
	add_subdirectory(apps/eventlogger)
	# oinputserver currently works only on Windows (uses WINSOCK)
	if(WIN32)
		add_subdirectory(apps/oinputserver)
	endif(WIN32)
endif(OMEGA_BUILD_APPS)

if(OMEGA_USE_DISPLAY)
	add_subdirectory(outils)
	if(OMEGA_BUILD_APPS)
		add_subdirectory(apps/ohello)
		add_subdirectory(apps/mocalib)
		add_subdirectory(apps/multipleKinectDemo)
		add_subdirectory(apps/meshviewer)
		add_subdirectory(apps/kinectdemo)
		add_subdirectory(apps/nightfield)
	endif(OMEGA_BUILD_APPS)
endif(OMEGA_USE_DISPLAY)

# vtk configuration, libraries and applications
if(OMEGA_BUILD_VTK_LIB)
	find_package(VTK)
	if(NOT VTK_DIR)
	  message(FATAL_ERROR "Please set VTK_DIR.")
	endif(NOT VTK_DIR)
	include(${VTK_USE_FILE})
	
	add_subdirectory(ovtk)
	if(OMEGA_BUILD_APPS)
		add_subdirectory(apps/vtkviewer)
	endif(OMEGA_BUILD_APPS)
endif(OMEGA_BUILD_VTK_LIB)

# Open Scene Graph configuration, libraries and applications
if(OMEGA_BUILD_OSG_LIB)
	find_package(OpenSceneGraph 2.9.8 COMPONENTS osgDB osgUtil)
	if(OSG_LIBRARY MATCHES "OSG_LIBRARY-NOTFOUND")
	  message(FATAL_ERROR "OSG not found")	 
	endif()
	if(${OPENSCENEGRAPH_VERSION} VERSION_LESS "2.9.8")
	  message(FATAL_ERROR "Wrong OSG version: ${OPENSCENEGRAPH_VERSION} found, need >= 2.9.8")
	endif()
	include_directories(SYSTEM ${OPENSCENEGRAPH_INCLUDE_DIRS})
	
	add_subdirectory(oosg)
	if(OMEGA_BUILD_APPS)
		add_subdirectory(apps/osgviewer)
	endif(OMEGA_BUILD_APPS)
endif(OMEGA_BUILD_OSG_LIB)

# Qt configuration, libraries and applications
if(OMEGA_BUILD_QT_LIB)
	# Find QT and provide a hint on where it might be
	find_package(Qt4 REQUIRED)
	if(${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} LESS 4.6)
		message(FATAL_ERROR "Qt version 4.6 or higher required. Found version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
	endif(${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} LESS 4.6)
	message(STATUS "Using Qt version "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.)
	set(QT_USE_QTUITOOLS TRUE)
	include(${QT_USE_FILE})
	include_directories(${QT_INCLUDES})
	link_directories(${QT_LIB_DIR})
	
	add_subdirectory(oqt)
	if(OMEGA_BUILD_APPS)
		add_subdirectory(apps/qtdemo)
	endif(OMEGA_BUILD_APPS)
endif(OMEGA_BUILD_QT_LIB)

