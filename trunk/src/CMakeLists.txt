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

###################################################################################################
# Options
set(OMEGA_BUILD_STATIC false CACHE BOOL "Build omega as a static library")
if(OMEGA_BUILD_STATIC)
	add_definitions(-DOMEGA_STATIC)
endif(OMEGA_BUILD_STATIC)

set(OMEGA_BUILD_APPS false CACHE BOOL "Enable building of omegalib applications. When disabled, only libraries will be built.")

set(OMEGA_USE_DISPLAY false CACHE BOOL "Enable display system support.")
if(OMEGA_USE_DISPLAY)
	set(OMEGA_USE_DISPLAY_EQUALIZER true CACHE BOOL "Enable Equalizer display system support")
	set(OMEGA_BUILD_ENGINE false CACHE BOOL "Enable to build the omegalib engine library (oengine). oengine is needed to support integration with external toolkits like vtk and Qt.")
	if(OMEGA_BUILD_ENGINE)
		set(OMEGA_BUILD_VTK_LIB false CACHE BOOL "Enable build of vtk integration library (Vtk has to be compiled with Python support)")
		set(OMEGA_BUILD_OSG_LIB false CACHE BOOL "Enable build of OpenSceneGraph integration library")
		set(OMEGA_BUILD_QT_LIB false CACHE BOOL "Enable build of Qt integration library")
	endif(OMEGA_BUILD_ENGINE)
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

if(OMEGA_USE_DISPLAY_EQUALIZER)
# Equalizer support enabled: uncompress and prepare the external project.
# PUT ALL OF THIS IN EXTERNAL SCRIPT
	ExternalProject_Add(
		equalizer
		URL ${CMAKE_SOURCE_DIR}/ext/equalizer.tar.gz
		#CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}
		CMAKE_ARGS 
			-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
			-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
			INSTALL_COMMAND ""
		)
	set_target_properties(equalizer PROPERTIES FOLDER "3rdparty")
	# define path to libraries built by the equalizer external project
	set(EQUALIZER_BINARY_DIR ${CMAKE_BINARY_DIR}/src/equalizer-prefix/src/equalizer-build)
	# NEED SECTIONS DEPENDENT ON BUILD TOOL, NOT OS!
	if(WIN32)
		set(EQUALIZER_EQ_LIB_DEBUG ${EQUALIZER_BINARY_DIR}/libs/client/Debug/Equalizer.lib)
		set(EQUALIZER_CO_LIB_DEBUG ${EQUALIZER_BINARY_DIR}/libs/collage/Debug/Collage.lib)
		set(EQUALIZER_LIBS_DEBUG debug ${EQUALIZER_EQ_LIB_DEBUG} debug ${EQUALIZER_CO_LIB_DEBUG})
		set(EQUALIZER_EQ_LIB_RELEASE ${EQUALIZER_BINARY_DIR}/libs/client/Release/Equalizer.lib)
		set(EQUALIZER_CO_LIB_RELEASE ${EQUALIZER_BINARY_DIR}/libs/collage/Release/Collage.lib)
		set(EQUALIZER_LIBS_RELEASE optimized ${EQUALIZER_EQ_LIB_RELEASE} optimized ${EQUALIZER_CO_LIB_RELEASE})
		# install(
			# FILES ${EQUALIZER_CLIENT_LIBS_DEBUG}
			# DESTINATION lib/Debug
			# COMPONENT omegalib
		# )
		# install(
			# FILES ${EQUALIZER_CLIENT_LIBS_RELEASE}
			# DESTINATION lib/Debug
			# COMPONENT omegalib
		# )
	else(WIN32)
		set(EQUALIZER_EQ_LIB_DEBUG ${EQUALIZER_BINARY_DIR}/libs/client/libEqualizer.so)
		set(EQUALIZER_CO_LIB_DEBUG ${EQUALIZER_BINARY_DIR}/libs/collage/libCollage.so)
		set(EQUALIZER_LIBS_DEBUG ${EQUALIZER_EQ_LIB_DEBUG} ${EQUALIZER_CO_LIB_DEBUG})
		set(EQUALIZER_LIBS_RELEASE )
		# install(
			# FILES ${EQUALIZER_CLIENT_LIBS_RELEASE}
			# DESTINATION lib
			# COMPONENT omegalib
		# )
	endif(WIN32)
	set(EQUALIZER_LIBS ${EQUALIZER_LIBS_DEBUG} ${EQUALIZER_LIBS_RELEASE})
	include_directories(${CMAKE_BINARY_DIR}/src/equalizer-prefix/src/equalizer-build/include)
	
endif(OMEGA_USE_DISPLAY_EQUALIZER)

###################################################################################################
# Set include paths
include_directories(
  ${OmegaLib_BINARY_DIR}/include/
  ${OmegaLib_SOURCE_DIR}/include/
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/libconfig  
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/glew  
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/ftgl/src/
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/freetype/include/
  ${OmegaLib_SOURCE_DIR}/src/3rdparty/FreeImage/Source/
  )

###################################################################################################
# Add libraries and applications to the project.
add_subdirectory(3rdparty/libconfig)
add_subdirectory(3rdparty/OpenThreads)

# TODO: should this be moved inside then omega OPENNI module block?
if(WIN32)
	include_directories($ENV{PROGRAMFILES}/OpenNI/Include)
endif(WIN32)

if(OMEGA_USE_DISPLAY)
	add_subdirectory(3rdparty/glew)
	if(OMEGA_BUILD_APPS)
		add_subdirectory(apps/ohello)
	endif(OMEGA_BUILD_APPS)
endif(OMEGA_USE_DISPLAY)

# Add some libraries used for display support.
if(OMEGA_BUILD_ENGINE)
	add_subdirectory(3rdparty/FreeImage)
	add_subdirectory(3rdparty/freetype)
	add_subdirectory(3rdparty/ftgl)
	add_subdirectory(oengine)
	if(OMEGA_BUILD_APPS)
		add_subdirectory(apps/mocalib)
		add_subdirectory(apps/multipleKinectDemo)
		add_subdirectory(apps/meshviewer)
		add_subdirectory(apps/kinectdemo)
	endif(OMEGA_BUILD_APPS)
endif(OMEGA_BUILD_ENGINE)

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

