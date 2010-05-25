#/********************************************************************************************************************** 
# * THE OMEGA LIB PROJECT
# *---------------------------------------------------------------------------------------------------------------------
# * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# * Authors:										
# *  [Author]									[Mail]
# *---------------------------------------------------------------------------------------------------------------------
# * [LICENSE NOTE]
# *---------------------------------------------------------------------------------------------------------------------
# * [SUMMARY OF FILE CONTENTS]
# *********************************************************************************************************************/

#######################################################################################################################
# Find required packages
find_package(GLUT)
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )

#######################################################################################################################
# Source files
SET( srcs 
		ocfg.c
        olist.c
        ogfx.c
        odisplay.c
        oevent.c
        oinput.c
        omega.c
        ostate.c
        othread.c
        outil.c
        olog.c
        display/ogl.c
        display/oeq.cpp
        input/omouse.c
        input/opq.c
        )
    
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/ocfg.h
        ${OmegaLib_SOURCE_DIR}/include/olist.h
        ${OmegaLib_SOURCE_DIR}/include/ogfx.h
        ${OmegaLib_SOURCE_DIR}/include/odisplay.h
        ${OmegaLib_SOURCE_DIR}/include/oevent.h
        ${OmegaLib_SOURCE_DIR}/include/oinput.h
        ${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/ostate.h
        ${OmegaLib_SOURCE_DIR}/include/othread.h
        ${OmegaLib_SOURCE_DIR}/include/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/outil.h
        ${OmegaLib_SOURCE_DIR}/include/olog.h
        display/ogl.h
        display/oeq.h
        input/omouse.h
        input/opq.h
        ) 
        
#######################################################################################################################
# Setup compile info
ADD_LIBRARY( omega ${srcs} ${headers})

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( omega
    libconfig
    pthreadVC2
    rpcrt4
    equalizer
    ${GLUT_LIBRARY} 
    ${OPENGL_LIBRARY}
)


