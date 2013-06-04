###################################################################################################
# THE OMEGA LIB PROJECT
#-------------------------------------------------------------------------------------------------
# Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
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
find_package(OpenGL REQUIRED)

if(OMEGA_BINARY_DIR)
	set(OMICRON_DEFAULT_BINARY_DIR ${OMEGA_BINARY_DIR}/omicron/omicron)
	#set(OMICRON_BIN_DIR ${OMEGA_BINARY_DIR}/bin)
	#set(OMICRON_LIB_DIR ${OMEGA_BINARY_DIR}/lib)
	include(${OMEGA_SOURCE_DIR}/external/omicron/CMakeModules/FindOmicron.cmake)

	# the following are the include directories needed to build a 3rd party omegalib application.
	# in the future, just ${OMEGA_ROOT_DIR}/include will be needed, but for now, multiple paths 
	# have to be specified. If building a project without Cmake, remember to specify ALL these directories
	# as include paths for your compiler.
	set(OMEGA_INCLUDE_DIRS ${OMICRON_INCLUDE_DIRS} ${OMEGA_BINARY_DIR}/include ${OMEGA_SOURCE_DIR}/include ${OMEGA_SOURCE_DIR}/external/include ${OSG_INCLUDES} ${PYTHON_INCLUDES})

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
		
		set(OMICRON_LIB_DEBUG ${OMEGA_LIB_DIR_DEBUG}/omicron.lib)
		set(OMICRON_LIB_RELEASE ${OMEGA_LIB_DIR_RELEASE}/omicron.lib)
		
		# omega
		set(OMEGA_LIB_DEBUG ${OMEGA_LIB_DIR_DEBUG}/omega.lib)
		set(OMEGA_LIB_RELEASE ${OMEGA_LIB_DIR_RELEASE}/omega.lib)
		
		# omegaToolkit
		set(OTK_LIB_DEBUG ${OMEGA_LIB_DIR_DEBUG}/omegaToolkit.lib)
		set(OTK_LIB_RELEASE ${OMEGA_LIB_DIR_RELEASE}/omegaToolkit.lib)
		
		# omegaOsg
		set(OOSG_LIB_DEBUG ${OMEGA_LIB_DIR_DEBUG}/omegaOsg.lib)
		set(OOSG_LIB_RELEASE ${OMEGA_LIB_DIR_RELEASE}/omegaOsg.lib)
		
		# omegaVtk
		set(OVTK_LIB_DEBUG ${OMEGA_LIB_DIR_DEBUG}/omegaVtk.lib)
		set(OVTK_LIB_RELEASE ${OMEGA_LIB_DIR_RELEASE}/omegaVtk.lib)
		
		# cyclops
		set(CY_LIB_DEBUG ${OMEGA_LIB_DIR_DEBUG}/cyclops.lib)
		set(CY_LIB_RELEASE ${OMEGA_LIB_DIR_RELEASE}/cyclops.lib)
		
		# glew
		#find_library(GLEW_LIB_DEBUG NAMES glew PATHS ${OMEGA_LIB_DIR_DEBUG})
		#find_library(GLEW_LIB_RELEASE NAMES glew PATHS ${OMEGA_LIB_DIR_RELEASE})
		
	else(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")
		set(OMICRON_LIB_DEBUG ${OMEGA_BIN_DIR}/libomicron.so)
		set(OMICRON_LIB_RELEASE ${OMEGA_BIN_DIR}/libomicron.so)
		
		# omega
		set(OMEGA_LIB_DEBUG ${OMEGA_BIN_DIR}/libomega.so)
		set(OMEGA_LIB_RELEASE ${OMEGA_BIN_DIR}/libomega.so)
		
		# omegaToolkit
		set(OTK_LIB_DEBUG ${OMEGA_BIN_DIR}/libomegaToolkit.so)
		set(OTK_LIB_RELEASE ${OMEGA_BIN_DIR}/libomegaToolkit.so)
		
		# oosg
		set(OOSG_LIB_DEBUG ${OMEGA_BIN_DIR}/libomegaOsg.so)
		set(OOSG_LIB_RELEASE ${OMEGA_BIN_DIR}/libomegaOsg.so)
		
		# oosg
		set(OVTK_LIB_DEBUG ${OMEGA_BIN_DIR}/libomegaVtk.so)
		set(OVTK_LIB_RELEASE ${OMEGA_BIN_DIR}/libomegaVtk.so)
		
		# cyclops
		set(CY_LIB_DEBUG ${OMEGA_BIN_DIR}/libcyclops.so)
		set(CY_LIB_RELEASE ${OMEGA_BIN_DIR}/libcyclops.so)
		
		# glew
		#find_library(GLEW_LIB_DEBUG NAMES glew PATHS ${OMEGA_LIB_DIR})
		#find_library(GLEW_LIB_RELEASE NAMES glew PATHS ${OMEGA_LIB_DIR})
	endif(MSVC OR CMAKE_GENERATOR STREQUAL "Xcode")

	#set(OMEGA_LIB debug ${OMEGA_LIB_DEBUG} debug ${OPENGL_LIBRARY} debug ${GLEW_LIB_DEBUG} debug ${LIBCONFIG_LIB_DEBUG} optimized ${OMEGA_LIB_RELEASE} optimized ${GLEW_LIB_RELEASE} optimized ${OPENGL_LIBRARY} optimized ${LIBCONFIG_LIB_RELEASE})
	set(OMEGA_LIB debug ${OMICRON_LIB_DEBUG} optimized ${OMICRON_LIB_RELEASE} debug ${OMEGA_LIB_DEBUG} debug ${OPENGL_LIBRARY} optimized ${OMEGA_LIB_RELEASE} optimized ${OPENGL_LIBRARY})
	set(OMEGA_TOOLKIT_LIB debug ${OTK_LIB_DEBUG} optimized ${OTK_LIB_RELEASE})
	set(OMEGA_OSG_LIB debug ${OOSG_LIB_DEBUG} optimized ${OOSG_LIB_RELEASE} ${OSG_LIBS})
	set(OMEGA_VTK_LIB debug ${OVTK_LIB_DEBUG} optimized ${OVTK_LIB_RELEASE})
	set(CYCLOPS_LIB debug ${CY_LIB_DEBUG} optimized ${CY_LIB_RELEASE})

	###################################################################################################
	# Visual studio specific options.
	if(MSVC)
		# Exclude libcmt when linking in visual studio
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
		set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
		add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4018)
	endif(MSVC)
endif(OMEGA_BINARY_DIR)

