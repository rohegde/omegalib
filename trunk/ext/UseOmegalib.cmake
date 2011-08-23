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
cmake_minimum_required(VERSION 2.8.1) 

if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
      #cmake_policy(SET CMP0008 NEW)
endif(COMMAND cmake_policy)

set(OMEGA_ROOT_DIR CACHE PATH "Path of the omegalib bin directory (the one containing the include, bin and lib folders")

###################################################################################################
# Create a build name depending on the OS, Toolset and architecture.
if(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")
	set(ARCH "win32")
	set(TOOL "vs9")
	add_definitions(-DOMEGA_OS_WIN -DOMEGA_ARCH_WIN32 -DOMEGA_TOOL_VS9)
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008 Win64")
	set(ARCH "win64")
	set(TOOL "vs9")
	add_definitions(-DOMEGA_OS_WIN -DOMEGA_ARCH_WIN64 -DOMEGA_TOOL_VS9)
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 10")
	set(ARCH "win32")
	set(TOOL "vs10")
	add_definitions(-DOMEGA_OS_WIN -DOMEGA_ARCH_WIN32 -DOMEGA_TOOL_VS10)
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")
	set(ARCH "win64")
	set(TOOL "vs10")
	add_definitions(-DOMEGA_OS_WIN -DOMEGA_ARCH_WIN64 -DOMEGA_TOOL_VS10)
elseif(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
	set(ARCH "linux-i386")
	set(TOOL "gcc4.4")
	add_definitions(-DOMEGA_OS_LINUX -DOMEGA_ARCH_I386 -DOMEGA_TOOL_GCC)
elseif(CMAKE_GENERATOR STREQUAL "Xcode")
	set(ARCH "OSX-i386")
	set(TOOL "gcc4.2")
	add_definitions(-DOMEGA_OS_OSX -DOMEGA_ARCH_I386 -DOMEGA_TOOL_GCC)
endif(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")

set(OMEGA_BUILD_NAME ${ARCH}-${TOOL})

# the following are the include directories needed to build a 3rd party omegalib application.
# in the future, just ${OMEGA_ROOT_DIR}/include will be needed, but for now, multiple paths 
# have to be specified. If building a project without Cmake, remember to specify ALL these directories
# as include paths for your compiler.
set(OMEGA_INCLUDE_DIR 
	${OMEGA_ROOT_DIR}/include
	${OMEGA_ROOT_DIR}/src/3rdparty/glew
	)
	  
set(OMEGA_LIB_DIR_RELEASE ${OMEGA_ROOT_DIR}/lib/${OMEGA_BUILD_NAME}-release)
set(OMEGA_LIB_DIR_DEBUG ${OMEGA_ROOT_DIR}/lib/${OMEGA_BUILD_NAME}-debug)
set(OMEGA_LIB_DIR ${OMEGA_ROOT_DIR}/lib/${OMEGA_BUILD_NAME})

set(OMEGA_BIN_DIR_RELEASE ${OMEGA_ROOT_DIR}/bin/${OMEGA_BUILD_NAME}-release)
set(OMEGA_BIN_DIR_DEBUG ${OMEGA_ROOT_DIR}/bin/${OMEGA_BUILD_NAME}-debug)
set(OMEGA_BIN_DIR ${OMEGA_ROOT_DIR}/bin/${OMEGA_BUILD_NAME})

###################################################################################################
# Set the output directories for libraries and binary files
if(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")
	# Since visual studio and Xcode builds are multiconfiguration, set two separate directories for debug and release builds
	find_library(OMEGA_LIB_DEBUG NAMES omega PATHS ${OMEGA_LIB_DIR_DEBUG})
	find_library(OMEGA_LIB_RELEASE NAMES omega PATHS ${OMEGA_LIB_DIR_RELEASE})
	find_library(OUTILS_LIB_DEBUG NAMES outils PATHS ${OMEGA_LIB_DIR_DEBUG})
	find_library(OUTILS_LIB_RELEASE NAMES outils PATHS ${OMEGA_LIB_DIR_RELEASE})
else(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")
	find_library(OMEGA_LIB_DEBUG NAMES omega PATHS ${OMEGA_LIB_DIR})
	find_library(OMEGA_LIB_RELEASE NAMES omega PATHS ${OMEGA_LIB_DIR})
	find_library(OUTILS_LIB_DEBUG NAMES omega PATHS ${OMEGA_LIB_DIR})
	find_library(OUTILS_LIB_RELEASE NAMES omega PATHS ${OMEGA_LIB_DIR})
endif(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")

###################################################################################################
# Visual studio specific options.
if(MSVC)
	# Exclude libcmt when linking in visual studio
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4018)
endif(MSVC)

include_directories(${OMEGA_INCLUDE_DIR})


