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
# When building in visual studio 10, use the NMake makefile generator for external projects.
# (Visual Studio 10 external generator has some speed / dependency resolution issues)
set(OSGWORKS_GENERATOR ${CMAKE_GENERATOR})
#if(OMEGA_TOOL_VS10)
#	set(OSGWORKS_GENERATOR "NMake Makefiles")
#endif(OMEGA_TOOL_VS10)

# Equalizer support enabled: uncompress and prepare the external project.
ExternalProject_Add(
	osgWorks
	URL ${CMAKE_SOURCE_DIR}/ext/osgw2.tar.gz
	#CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}
	CMAKE_GENERATOR ${OSGWORKS_GENERATOR}
	CMAKE_ARGS 
		-DCMAKE_SHARED_LINKER_FLAGS:STRING="${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:msvcprt.lib /NODEFAULTLIB:libcpmt.lib"
		-DCMAKE_LINKER_FLAGS:STRING="${CMAKE_LINKER_FLAGS} /NODEFAULTLIB:libcpmt.lib /NODEFAULTLIB:msvcprt.lib"
		-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
		-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
		-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
		-DOSGInstallType:STRING="Alternate Install Location"
		-DOSGInstallLocation:PATH=${OSG_BINARY_DIR}
		#osg
		-DOSG_INCLUDE_DIR:PATH=${osg_INCLUDE_DIR}
		-DOSG_LIBRARY:PATH=${osg_LIBRARY}
		-DOSG_LIBRARY_DEBUG:PATH=${osg_LIBRARY_DEBUG}
		#osgGA
		-DOSGGA_INCLUDE_DIR:PATH=${osgGA_INCLUDE_DIR}
		-DOSGGA_LIBRARY:PATH=${osgGA_LIBRARY}
		-DOSGGA_LIBRARY_DEBUG:PATH=${osgGA_LIBRARY_DEBUG}
		#osgText
		-DOSGTEXT_INCLUDE_DIR:PATH=${osgText_INCLUDE_DIR}
		-DOSGTEXT_LIBRARY:PATH=${osgText_LIBRARY}
		-DOSGTEXT_LIBRARY_DEBUG:PATH=${osgText_LIBRARY_DEBUG}
		#osgViewer
		-DOSGVIEWER_INCLUDE_DIR:PATH=${osgViewer_INCLUDE_DIR}
		-DOSGVIEWER_LIBRARY:PATH=${osgViewer_LIBRARY}
		-DOSGVIEWER_LIBRARY_DEBUG:PATH=${osgViewer_LIBRARY_DEBUG}
		#osgSim
		-DOSGSIM_INCLUDE_DIR:PATH=${osgSim_INCLUDE_DIR}
		-DOSGSIM_LIBRARY:PATH=${osgSim_LIBRARY}
		-DOSGSIM_LIBRARY_DEBUG:PATH=${osgSim_LIBRARY_DEBUG}
		#osgDB
		-DOSGDB_INCLUDE_DIR:PATH=${osgDB_INCLUDE_DIR}
		-DOSGDB_LIBRARY:PATH=${osgDB_LIBRARY}
		-DOSGDB_LIBRARY_DEBUG:PATH=${osgDB_LIBRARY_DEBUG}
		#osgUtil
		-DOSGUTIL_INCLUDE_DIR:PATH=${osgUtil_INCLUDE_DIR}
		-DOSGUTIL_LIBRARY:PATH=${osgUtil_LIBRARY}
		-DOSGUTIL_LIBRARY_DEBUG:PATH=${osgUtil_LIBRARY_DEBUG}
		#openthreads
		-DOPENTHREADS_INCLUDE_DIR:PATH=${OpenThreads_INCLUDE_DIR}
		-DOPENTHREADS_LIBRARY:PATH=${OpenThreads_LIBRARY}
		-DOPENTHREADS_LIBRARY_DEBUG:PATH=${OpenThreads_LIBRARY_DEBUG}
		INSTALL_COMMAND ""
	)
set_target_properties(osgWorks PROPERTIES FOLDER "3rdparty")
# define path to libraries built by the equalizer external project
set(OSGWORKS_BINARY_DIR ${CMAKE_BINARY_DIR}/src/osgWorks-prefix/src/osgWorks-build/lib)
set(OSGWORKS_COMPONENTS osgwTools osgwQuery)

# NEED SECTIONS DEPENDENT ON BUILD TOOL, NOT OS!
if(OMEGA_OS_WIN)
    foreach( C ${OSGWORKS_COMPONENTS} )
		set(${C}_LIBRARY ${OSGWORKS_BINARY_DIR}/${C}.lib)
		set(${C}_LIBRARY_DEBUG ${OSGWORKS_BINARY_DIR}/${C}d.lib)
		set(OSGWORKS_LIBS ${OSGWORKS_LIBS} optimized ${${C}_LIBRARY} debug ${${C}_LIBRARY_DEBUG})
	endforeach()
elseif(OMEGA_OS_LINUX)
    foreach( C ${OSGWORKS_COMPONENTS} )
		set(${C}_LIBRARY ${OSGWORKS_BINARY_DIR}/lib${C}.a)
		set(${C}_LIBRARY_DEBUG ${OSGWORKS_BINARY_DIR}/lib${C}d.a)
		set(OSGWORKS_LIBS ${OSGWORKS_LIBS} optimized ${${C}_LIBRARY} debug ${${C}_LIBRARY_DEBUG})
	endforeach()
endif(OMEGA_OS_WIN)

add_definitions(-DOSGWORKS_STATIC)

set(OSGWORKS_INCLUDES ${CMAKE_BINARY_DIR}/src/osgWorks-prefix/src/osgWorks/include)
