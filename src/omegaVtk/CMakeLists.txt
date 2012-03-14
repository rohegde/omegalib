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

###################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DOVTK_EXPORTING -DVTK_LIBRARY_DIR="${VTK_LIBRARY_DIRS}" -DVTK_PYTHON_DIR="${VTK_LIBRARY_DIRS}/../Wrapping/Python")

###################################################################################################
# Source files
SET( srcs 
		omegaVtk/vtkGenericOpenGLRenderWindow.cpp
		omegaVtk/VtkModule.cpp
		omegaVtk/VtkRenderPass.cpp
		omegaVtk/VtkAttachPoint.cpp
        )
		
if(OMEGA_USE_PYTHON)
SET( srcs ${srcs}
		omegaVtk/PythonUIEventHandler.cpp
		omegaVtk/PyVtk.cpp
		omegaVtk/omegaVtkPythonApi.cpp
        )
endif()
		
###################################################################################################
# Headers
SET( headers 
		# omega
		${CMAKE_SOURCE_DIR}/include/omegaVtk/ovtkbase.h
		${CMAKE_SOURCE_DIR}/include/omegaVtk/PythonUIEventHandler.h
		${CMAKE_SOURCE_DIR}/include/omegaVtk/PyVtk.h
		${CMAKE_SOURCE_DIR}/include/omegaVtk/VtkModule.h
		${CMAKE_SOURCE_DIR}/include/omegaVtk/vtkGenericOpenGLRenderWindow.h
		${CMAKE_SOURCE_DIR}/include/omegaVtk/VtkRenderPass.h
		${CMAKE_SOURCE_DIR}/include/omegaVtk/VtkAttachPoint.h
		${CMAKE_SOURCE_DIR}/include/omegaVtk/omegaVtkPythonApi.h
        ) 
        
###################################################################################################
SOURCE_GROUP(omegaVtk REGULAR_EXPRESSION omegaVtk/)		

###################################################################################################
# Setup compile info
IF(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( omegaVtk ${srcs} ${headers})
ELSE(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( omegaVtk SHARED ${srcs} ${headers})
ENDIF(OMEGA_BUILD_STATIC)

###################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( omegaVtk 
	omega 
	omegaToolkit
    vtkRendering
	)
