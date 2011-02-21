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
set_property(GLOBAL PROPERTY USE_FOLDERS On)
cmake_minimum_required(VERSION 2.8.2) 

project(OmegaLib)

if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
      #cmake_policy(SET CMP0008 NEW)
endif(COMMAND cmake_policy)

###################################################################################################
# Create a build name depending on the OS, Toolset and architecture.
if(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")
	set(ARCH "win32")
	set(TOOL "vs9")
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008 Win64")
	set(ARCH "win64")
	set(TOOL "vs9")
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 10")
	set(ARCH "win32")
	set(TOOL "vs10")
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")
	set(ARCH "win64")
	set(TOOL "vs10")
elseif(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
	set(ARCH "linux-i386")
	set(TOOL "gcc4.4")
endif(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")

set(OMEGA_BUILD_NAME ${ARCH}-${TOOL} CACHE STRING "The omegalib build name. A directory with this name will be created in the bin and lib directories to store the final build products.")

###################################################################################################
# Set the output directories for libraries and binary files
if(MSVC)
	# Since visual studio builds are multiconfiguration, set two separate directories for debug and release builds
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OmegaLib_SOURCE_DIR}/lib/${OMEGA_BUILD_NAME}-debug)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OmegaLib_SOURCE_DIR}/lib/${OMEGA_BUILD_NAME}-release)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OmegaLib_SOURCE_DIR}/lib/${OMEGA_BUILD_NAME}-debug)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OmegaLib_SOURCE_DIR}/lib/${OMEGA_BUILD_NAME}-release)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OmegaLib_SOURCE_DIR}/bin/${OMEGA_BUILD_NAME}-debug)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OmegaLib_SOURCE_DIR}/bin/${OMEGA_BUILD_NAME}-release)
	# Make sure the output bin directories exist, so the cmake scripts can copy 
	# third party dlls there if needed.
	file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
	file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
else(MSVC)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OmegaLib_SOURCE_DIR}/lib/${OMEGA_BUILD_NAME})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OmegaLib_SOURCE_DIR}/lib/${OMEGA_BUILD_NAME})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OmegaLib_SOURCE_DIR}/bin/${OMEGA_BUILD_NAME})
endif(MSVC)

###################################################################################################
# Couple utility variables for library linking
#SET (LIB_DEBUG ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-debug)
#SET (LIB_RELEASE ${PROJECT_SOURCE_DIR}/lib/${ARCH}-${TOOL}-release)

###################################################################################################
# Visual studio specific options.
if(MSVC)
	# Exclude libcmt when linking in visual studio
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4018)
endif(MSVC)

###################################################################################################
# Add subdirectiories
add_subdirectory(src)
