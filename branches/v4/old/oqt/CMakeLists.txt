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

###################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DOQT_EXPORTING)

###################################################################################################
# Source files
SET( srcs 
		oqt/QtWidgetManager.cpp
		oqt/QtWidget.cpp
        )
		
###################################################################################################
# Headers
SET( headers 
		# omega
		${OmegaLib_SOURCE_DIR}/include/oqt/oqtbase.h
		${OmegaLib_SOURCE_DIR}/include/oqt/QtWidgetManager.h
		${OmegaLib_SOURCE_DIR}/include/oqt/QtWidget.h
        ) 
        
qt4_wrap_cpp(OQT_MOC_SOURCES ${headers})

###################################################################################################
source_group(oqt REGULAR_EXPRESSION oqt/)		
source_group( Moc REGULAR_EXPRESSION moc_.*)

###################################################################################################
# Setup compile info
if(OMEGA_BUILD_STATIC)
	add_library( oqt ${srcs} ${headers} ${OQT_MOC_SOURCES})
else(OMEGA_BUILD_STATIC)
	add_library( oqt SHARED ${srcs} ${headers} ${OQT_MOC_SOURCES})
endif(OMEGA_BUILD_STATIC)

###################################################################################################
# Setup link info.
target_link_libraries( oqt 
    ${QT_LIBRARIES}
	omega 
	outils
	)
