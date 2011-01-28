#********************************************************************************************************************** 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#   Alessandro Febretti							febret@gmail.com
#---------------------------------------------------------------------------------------------------------------------
# [LICENSE NOTE]
#---------------------------------------------------------------------------------------------------------------------
# Cmake file for outils
#*********************************************************************************************************************/

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DLIBCONFIGXX_STATIC -DOUTILS_EXPORTING -DFTGL_LIBRARY_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC)

#######################################################################################################################
# Source files
SET( srcs 
		# omega
		omega/FontManager.cpp
		omega/TextureManager.cpp
		
		# omega::scene
		omega/scene/Effect.cpp
		omega/scene/EffectManager.cpp
		omega/scene/Mesh.cpp
		omega/scene/MeshData.cpp
		omega/scene/MeshManager.cpp
		omega/scene/Node.cpp
		omega/scene/SimplePrimitive.cpp
		omega/scene/SceneManager.cpp
		omega/scene/SceneQuery.cpp
		omega/scene/SceneNode.cpp
		omega/scene/PlyDataReader.cpp
		omega/scene/plyfile.cpp
		
		# omega::ui
		omega/ui/AbstractButton.cpp
		omega/ui/Box.cpp
		omega/ui/Button.cpp
		omega/ui/Image.cpp
		omega/ui/Label.cpp
		omega/ui/DefaultSkin.cpp
		omega/ui/Slider.cpp
		omega/ui/UIManager.cpp
		omega/ui/Widget.cpp
        )
		
#######################################################################################################################
# Headers
SET( headers 
		# omega
		${OmegaLib_SOURCE_DIR}/include/omega/FontManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/TextureManager.h
		
		# omega::scene
		${OmegaLib_SOURCE_DIR}/include/omega/scene.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Mesh.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/MeshData.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/MeshManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Node.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Effect.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/EffectManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Drawable.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SimplePrimitive.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneQuery.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneNode.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/PlyDataReader.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/ply.h
		
		# omega::ui
		${OmegaLib_SOURCE_DIR}/include/omega/ui.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/AbstractButton.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Box.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Button.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Image.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Label.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/DefaultSkin.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Slider.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/UIManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Widget.h
        ) 
        
#######################################################################################################################
# Enable / disable specific modules within omegalib

#######################################################################################################################
SOURCE_GROUP(omega REGULAR_EXPRESSION omega/)		
SOURCE_GROUP(omega::ui REGULAR_EXPRESSION omega/ui/)		
SOURCE_GROUP(omega::scene REGULAR_EXPRESSION omega/scene/)		

#######################################################################################################################
# Setup compile info
IF(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( outils ${srcs} ${headers})
ELSE(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( outils SHARED ${srcs} ${headers})
ENDIF(OMEGA_BUILD_STATIC)

#######################################################################################################################
# Setup module-specific link info

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( outils
	omega
	debug ${LIB_DEBUG}/freetype2312_D.lib optimized ${LIB_RELEASE}/freetype2312.lib
	debug ${LIB_DEBUG}/ftgl_static_D.lib optimized ${LIB_RELEASE}/ftgl_static.lib
	debug ${LIB_DEBUG}/FreeImage.lib optimized ${LIB_RELEASE}/FreeImage.lib
	)
