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
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
      #cmake_policy(SET CMP0008 NEW)
endif(COMMAND cmake_policy)

set(OMEGA_BINARY_DIR CACHE PATH "Path of the omegalib bin directory (the one containing the include, bin and lib folders")

if(OMEGA_BINARY_DIR)
	include(${OMEGA_BINARY_DIR}/UseOmegalib.cmake)

	# the following are the include directories needed to build a 3rd party omegalib application.
	# in the future, just ${OMEGA_ROOT_DIR}/include will be needed, but for now, multiple paths 
	# have to be specified. If building a project without Cmake, remember to specify ALL these directories
	# as include paths for your compiler.
	set(OMEGA_INCLUDE_DIRS ${OMEGA_BINARY_DIR}/include ${OMEGA_SOURCE_DIR}/include ${OMEGA_SOURCE_DIR}/ext/windows/include)
		  
	set(OMEGA_LIB_DIR_RELEASE ${OMEGA_BINARY_DIR}/lib/release)
	set(OMEGA_LIB_DIR_DEBUG ${OMEGA_BINARY_DIR}/lib/debug)
	set(OMEGA_LIB_DIR ${OMEGA_BINARY_DIR}/lib)

	set(OMEGA_BIN_DIR_RELEASE ${OMEGA_BINARY_DIR}/bin/release)
	set(OMEGA_BIN_DIR_DEBUG ${OMEGA_BINARY_DIR}/bin/debug)
	set(OMEGA_BIN_DIR ${OMEGA_BINARY_DIR}/bin)

	###################################################################################################
	# Set the output directories for libraries and binary files
	if(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")
		# Since visual studio and Xcode builds are multiconfiguration, set two separate directories for debug and release builds
		
		# omega
		find_library(OMEGA_LIB_DEBUG NAMES omega PATHS ${OMEGA_LIB_DIR_DEBUG})
		find_library(OMEGA_LIB_RELEASE NAMES omega PATHS ${OMEGA_LIB_DIR_RELEASE})
		
		# oengine
		find_library(OENGINE_LIB_DEBUG NAMES oengine PATHS ${OMEGA_LIB_DIR_DEBUG})
		find_library(OENGINE_LIB_RELEASE NAMES oengine PATHS ${OMEGA_LIB_DIR_RELEASE})
		
		# libconfig
		find_library(LIBCONFIG_LIB_DEBUG NAMES libconfig PATHS ${OMEGA_LIB_DIR_DEBUG})
		find_library(LIBCONFIG_LIB_RELEASE NAMES libconfig PATHS ${OMEGA_LIB_DIR_RELEASE})
		
	else(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")
		# omega
		find_library(OMEGA_LIB_DEBUG NAMES omega PATHS ${OMEGA_BIN_DIR})
		find_library(OMEGA_LIB_RELEASE NAMES omega PATHS ${OMEGA_BIN_DIR})
		
		# oengine
		find_library(OENGINE_LIB_DEBUG NAMES oengine PATHS ${OMEGA_BIN_DIR})
		find_library(OENGINE_LIB_RELEASE NAMES oengine PATHS ${OMEGA_BIN_DIR})
		
		# libconfig
		find_library(LIBCONFIG_LIB_DEBUG NAMES libconfig PATHS ${OMEGA_BIN_DIR})
		find_library(LIBCONFIG_LIB_RELEASE NAMES libconfig PATHS ${OMEGA_BIN_DIR})
	endif(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")

	set(OMEGA_LIB debug ${OMEGA_LIB_DEBUG} ${OPENGL_LIBRARY} ${LIBCONFIG_LIB_DEBUG} optimized ${OMEGA_LIB_RELEASE} ${OPENGL_LIBRARY} ${LIBCONFIG_LIB_RELEASE})
	set(OENGINE_LIB debug ${OENGINE_LIB_DEBUG} ${OPENGL_LIBRARY} optimized ${OENGINE_LIB_RELEASE} ${OPENGL_LIBRARY})

	###################################################################################################
	# Visual studio specific options.
	if(MSVC)
		# Exclude libcmt when linking in visual studio
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
		set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
		add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4018)
	endif(MSVC)
endif(OMEGA_BINARY_DIR)

