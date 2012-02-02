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
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
include(ExternalProject)
include(OmegaAddTutorial)
include(UsePch)

include(${OmegaLib_SOURCE_DIR}/external/UseOmicron.cmake)

###################################################################################################
# Options
set(OMEGA_BUILD_TUTORIALS false CACHE BOOL "Enable building of omegalib tutorials.")
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

# Add the core omega project
add_subdirectory(omega)

add_subdirectory(3rdparty/glew)
add_subdirectory(apps/ohello)
if(OMEGA_BUILD_TUTORIALS)
	omega_add_tutorial(tutorial4)
endif(OMEGA_BUILD_TUTORIALS)

add_subdirectory(omegaToolkit)

add_subdirectory(apps/meshviewer)
add_subdirectory(apps/nightfield)

# Add a couple of applications that require OpenNI support.
if(OMICRON_USE_OPENNI)
	add_subdirectory(apps/kinectdemo)
	add_subdirectory(apps/mocalib)
endif()

if(OMEGA_BUILD_TUTORIALS)
	omega_add_tutorial(tutorial5)
	omega_add_tutorial(tutorial6)
endif()

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
	add_subdirectory(apps/cyclops)
	add_subdirectory(apps/osgviewer)
	
	if(OMEGA_BUILD_TUTORIALS)
		# osgshape tutorial
		omega_add_tutorial(tutorial7) 
	endif()
endif()

###################################################################################################
# EXPERIMENTAL LIBS
# vtk configuration, libraries and applications
if(OMEGA_BUILD_VTK_LIB)
	find_package(VTK)
	if(NOT VTK_DIR)
	  message(FATAL_ERROR "Please set VTK_DIR.")
	endif(NOT VTK_DIR)
	include(${VTK_USE_FILE})
	
	add_subdirectory(ovtk)
	#if(OMEGA_BUILD_APPS)
		#add_subdirectory(apps/vtkviewer)
	#endif(OMEGA_BUILD_APPS)
endif(OMEGA_BUILD_VTK_LIB)

configure_file(${CMAKE_SOURCE_DIR}/CMakeModules/UseOmegalib.cmake.in ${CMAKE_BINARY_DIR}/UseOmegalib.cmake)

