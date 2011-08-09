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
# Options
set(OMEGA_BUILD_STATIC false CACHE BOOL "Build omega as a static library")
if(OMEGA_BUILD_STATIC)
	add_definitions(-DOMEGA_STATIC)
endif(OMEGA_BUILD_STATIC)

set(APPS_BUILD_INPUT_SERVER false CACHE BOOL "Build omegalib input server")
set(APPS_BUILD_MOCALIB false CACHE BOOL "Build motion capture calibration app")

set(OMEGA_USE_DISPLAY false CACHE BOOL "Enable display system support.")
if(OMEGA_USE_DISPLAY)
	set(OMEGA_BUILD_UTILITY_LIB false CACHE BOOL "Enable build of the omegalib utility library (outils)")
endif(OMEGA_USE_DISPLAY)

if(OMEGA_BUILD_UTILITY_LIB)
	set(APPS_BUILD_MESH_VIEWER false CACHE BOOL "Enable build of demo applications included in Omegalib")
	set(APPS_BUILD_KINECT false CACHE BOOL "Enable build of kinect demo")
	set(OMEGA_BUILD_VTK_LIB false CACHE BOOL "Enable build of vtk integration library (Vtk has to be compiled with Python support)")
	set(OMEGA_BUILD_OSG_LIB false CACHE BOOL "Enable build of OpenSceneGraph integration library")
	set(OMEGA_BUILD_QT_LIB false CACHE BOOL "Enable build of Qt integration library")
endif(OMEGA_BUILD_UTILITY_LIB)

if(OMEGA_BUILD_VTK_LIB)
	set(APPS_BUILD_VTK_VIEWER false CACHE BOOL "Enable build of vtk viewer application ")
endif(OMEGA_BUILD_VTK_LIB)

if(OMEGA_BUILD_OSG_LIB)
	set(APPS_BUILD_OSG_VIEWER false CACHE BOOL "Enable build of osg viewer application ")
endif(OMEGA_BUILD_OSG_LIB)

if(OMEGA_BUILD_QT_LIB)
	set(APPS_BUILD_QT_DEMO false CACHE BOOL "Enable build of qt demo application ")
endif(OMEGA_BUILD_QT_LIB)

if(OMEGA_USE_OPENCL)
	set(APPS_BUILD_NIGHTFIELD false CACHE BOOL "Enable build of nightfield application ")
endif(OMEGA_USE_OPENCL)

###################################################################################################
# Add subdirectiories
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
  
if(WIN32)
	include_directories($ENV{PROGRAMFILES}/OpenNI/Include)
endif(WIN32)

add_subdirectory(3rdparty/libconfig)
add_subdirectory(3rdparty/equalizer)

if(OMEGA_BUILD_UTILITY_LIB)
	add_subdirectory(3rdparty/FreeImage)
	add_subdirectory(3rdparty/freetype)
	add_subdirectory(3rdparty/ftgl)
endif(OMEGA_BUILD_UTILITY_LIB)

if( OMEGA_USE_DISPLAY_GLUT) 
    if(NOT APPLE) 
	    add_subdirectory(3rdparty/freeglut)
    endif(NOT APPLE)
endif(OMEGA_USE_DISPLAY_GLUT)

if(OMEGA_USE_DISPLAY)
	add_subdirectory(3rdparty/glew)
endif(OMEGA_USE_DISPLAY)

# Add the core omega project
add_subdirectory(omega)

# Add the basic test app for omegalib (a simple input event logger)
add_subdirectory(apps/eventlogger)

# Check if 32bit MAC or 64bit MAC
if( APPLE )
    if(_CMAKE_OSX_MACHINE MATCHES "64")
        option(EQUALIZER_PREFER_AGL "Prefer AGL over GLX on Mac OS X" OFF)
    endif()
endif( APPLE )

if(OMEGA_BUILD_UTILITY_LIB)
	add_subdirectory(outils)
endif(OMEGA_BUILD_UTILITY_LIB)

if(OMEGA_BUILD_VTK_LIB)
	add_subdirectory(ovtk)
endif(OMEGA_BUILD_VTK_LIB)

if(OMEGA_BUILD_OSG_LIB)
	add_subdirectory(oosg)
endif(OMEGA_BUILD_OSG_LIB)

if(OMEGA_BUILD_QT_LIB)
	######################################################################
	# Configure Qt													     #
	######################################################################

	# Find QT and provide a hint on where it might be
	find_package(Qt4 REQUIRED)

	if(${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} LESS 4.6)
		message(FATAL_ERROR "Qt version 4.6 or higher required. Found version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
	endif(${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} LESS 4.6)

	message("Using Qt version "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.)

	set(QT_USE_QTUITOOLS TRUE)
	include(${QT_USE_FILE})
	include_directories(${QT_INCLUDES})
	link_directories(${QT_LIB_DIR})
	
	add_subdirectory(oqt)
endif(OMEGA_BUILD_QT_LIB)

if(OMEGA_USE_DISPLAY) 
	add_subdirectory(apps/ohello)
endif(OMEGA_USE_DISPLAY)

if(APPS_BUILD_INPUT_SERVER)
	add_subdirectory(apps/oinputserver)
endif(APPS_BUILD_INPUT_SERVER)

if(APPS_BUILD_MOCALIB)
	add_subdirectory(apps/mocalib)
endif(APPS_BUILD_MOCALIB)

if(APPS_BUILD_MOCALIB)
	ADD_SUBDIRECTORY(apps/multipleKinectDemo)
endif(APPS_BUILD_MOCALIB)

if(APPS_BUILD_MESH_VIEWER)
	add_subdirectory(apps/meshviewer)
endif(APPS_BUILD_MESH_VIEWER)

if(APPS_BUILD_KINECT)
	add_subdirectory(apps/kinectdemo)
endif(APPS_BUILD_KINECT)

if(APPS_BUILD_VTK_VIEWER)
	add_subdirectory(apps/vtkviewer)
endif(APPS_BUILD_VTK_VIEWER)

if(APPS_BUILD_OSG_VIEWER)
	add_subdirectory(apps/osgviewer)
endif(APPS_BUILD_OSG_VIEWER)

if(APPS_BUILD_QT_DEMO)
	add_subdirectory(apps/qtdemo)
endif(APPS_BUILD_QT_DEMO)

if(APPS_BUILD_NIGHTFIELD)
	add_subdirectory(apps/nightfield)
endif(APPS_BUILD_NIGHTFIELD)
