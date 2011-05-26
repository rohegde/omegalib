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
endif(OMEGA_BUILD_UTILITY_LIB)

if(OMEGA_BUILD_VTK_LIB)
	set(APPS_BUILD_VTK_VIEWER false CACHE BOOL "Enable build of vtk viewer application ")
endif(OMEGA_BUILD_VTK_LIB)

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
  ${OmegaLib_SOURCE_DIR}/ext/windows/include/
 ) 
  
add_subdirectory(3rdparty/libconfig)
add_subdirectory(3rdparty/equalizer)

if(OMEGA_BUILD_UTILITY_LIB)
	add_subdirectory(3rdparty/FreeImage)
	add_subdirectory(3rdparty/freetype)
	add_subdirectory(3rdparty/ftgl)
endif(OMEGA_BUILD_UTILITY_LIB)

if( OMEGA_USE_DISPLAY_GLUT) 
	add_subdirectory(3rdparty/freeglut)
endif(OMEGA_USE_DISPLAY_GLUT)

if(OMEGA_USE_DISPLAY)
	add_subdirectory(3rdparty/glew)
endif(OMEGA_USE_DISPLAY)

add_subdirectory(omega)

if(OMEGA_BUILD_UTILITY_LIB)
	add_subdirectory(outils)
endif(OMEGA_BUILD_UTILITY_LIB)

if(OMEGA_BUILD_VTK_LIB)
	add_subdirectory(ovtk)
endif(OMEGA_BUILD_VTK_LIB)

IF(OMEGA_USE_DISPLAY) 
add_subdirectory(apps/ohello)
ENDIF(OMEGA_USE_DISPLAY)

add_subdirectory(apps/eventlogger)

if(APPS_BUILD_INPUT_SERVER)
	ADD_SUBDIRECTORY(apps/oinputserver)
endif(APPS_BUILD_INPUT_SERVER)

if(APPS_BUILD_MOCALIB)
	ADD_SUBDIRECTORY(apps/mocalib)
endif(APPS_BUILD_MOCALIB)

if(APPS_BUILD_MESH_VIEWER)
	add_subdirectory(apps/meshviewer)
endif(APPS_BUILD_MESH_VIEWER)

IF(APPS_BUILD_KINECT)
	ADD_SUBDIRECTORY(apps/kinectdemo)
ENDIF(APPS_BUILD_KINECT)

IF(APPS_BUILD_VTK_VIEWER)
	ADD_SUBDIRECTORY(apps/vtkviewer)
ENDIF(APPS_BUILD_VTK_VIEWER)

IF(APPS_BUILD_NIGHTFIELD)
	ADD_SUBDIRECTORY(apps/nightfield)
ENDIF(APPS_BUILD_NIGHTFIELD)
