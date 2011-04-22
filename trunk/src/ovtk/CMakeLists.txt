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
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF(NOT VTK_DIR)
INCLUDE(${VTK_USE_FILE})

###################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DOVTK_EXPORTING -DVTK_LIBRARY_DIR="${VTK_LIBRARY_DIRS}" -DVTK_PYTHON_DIR="${VTK_LIBRARY_DIRS}/../Wrapping/Python")

###################################################################################################
# Source files
SET( srcs 
		ovtk/PythonInterpreter.cpp
		ovtk/PyVtk.cpp
		ovtk/VtkClient.cpp
		ovtk/VtkEntity.cpp
		ovtk/VtkRenderable.cpp
		ovtk/vtkGenericOpenGLRenderWindow.cpp
		ovtk/VtkRenderPass.cpp
        )
		
###################################################################################################
# Headers
SET( headers 
		# omega
		${OmegaLib_SOURCE_DIR}/include/ovtk/ovtkbase.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/PythonInterpreter.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/PythonInterpreterWrapper.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/PyVtk.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/VtkClient.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/VtkEntity.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/VtkRenderable.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/vtkGenericOpenGLRenderWindow.h
		${OmegaLib_SOURCE_DIR}/include/ovtk/VtkRenderPass.h
        ) 
        
###################################################################################################
SOURCE_GROUP(ovtk REGULAR_EXPRESSION ovtk/)		

###################################################################################################
# Setup compile info
IF(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( ovtk ${srcs} ${headers})
ELSE(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( ovtk SHARED ${srcs} ${headers})
ENDIF(OMEGA_BUILD_STATIC)

###################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( ovtk 
	omega 
	outils
    vtkRendering
	)
