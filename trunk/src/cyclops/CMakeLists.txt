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
add_definitions( -D_CRT_SECURE_NO_WARNINGS -DCY_EXPORTING)
set(SRCS 
		SceneLoader.cpp
		SceneManager.cpp
		cyclopsPythonApi.cpp)
    
set(OSG_SRCS 
		osg/obj.cpp
		osg/plyfile.cpp
		osg/ReaderWriterPLY.cpp
		osg/vertexData.cpp
		osg/ReaderXOBJ.cpp)
		
set(HEADERS 
		${CMAKE_SOURCE_DIR}/include/cyclops/cyclopsConfig.h
		${CMAKE_SOURCE_DIR}/include/cyclops/SceneLoader.h
		${CMAKE_SOURCE_DIR}/include/cyclops/SceneManager.h)
		
###################################################################################################
# Setup compile info
if(OMEGA_BUILD_STATIC)
	add_library( cyclops ${SRCS} ${OSG_SRCS} ${HEADERS})
else(OMEGA_BUILD_STATIC)
	add_library( cyclops SHARED ${SRCS} ${OSG_SRCS} ${HEADERS})
endif(OMEGA_BUILD_STATIC)

source_group(osg REGULAR_EXPRESSION osg/)		
target_link_libraries(cyclops
	omega 
	omegaToolkit
    omegaOsg)
