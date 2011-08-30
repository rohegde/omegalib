set(ftgl_SOURCES
    src/FTBuffer.cpp 
    src/FTCharmap.cpp 
    src/FTCharmap.h 
    src/FTCharToGlyphIndexMap.h 
    src/FTCleanup.cpp 
    src/FTCleanup.h 
    src/FTContour.cpp 
    src/FTContour.h 
    src/FTFace.cpp 
    src/FTFace.h 
    src/FTGL.cpp 
    src/FTGlyphContainer.cpp 
    src/FTGlyphContainer.h 
    src/FTInternals.h 
    src/FTLibrary.cpp 
    src/FTLibrary.h 
    src/FTList.h 
    src/FTPoint.cpp 
    src/FTSize.cpp 
    src/FTSize.h 
    src/FTVector.h 
    src/FTVectoriser.cpp 
    src/FTVectoriser.h 
    src/FTUnicode.h 
	
    src/FTGL/ftgl.h 
    src/FTGL/FTBBox.h 
    src/FTGL/FTBuffer.h 
    src/FTGL/FTPoint.h 
    src/FTGL/FTGlyph.h 
    src/FTGL/FTBitmapGlyph.h 
    src/FTGL/FTBufferGlyph.h 
    src/FTGL/FTExtrdGlyph.h 
    src/FTGL/FTOutlineGlyph.h 
    src/FTGL/FTPixmapGlyph.h 
    src/FTGL/FTPolyGlyph.h 
    src/FTGL/FTTextureGlyph.h 
    src/FTGL/FTFont.h 
    src/FTGL/FTGLBitmapFont.h 
    src/FTGL/FTBufferFont.h 
    src/FTGL/FTGLExtrdFont.h 
    src/FTGL/FTGLOutlineFont.h 
    src/FTGL/FTGLPixmapFont.h 
    src/FTGL/FTGLPolygonFont.h 
    src/FTGL/FTGLTextureFont.h 
    src/FTGL/FTLayout.h 
    src/FTGL/FTSimpleLayout.h 
	
    src/FTGlyph/FTGlyph.cpp 
    src/FTGlyph/FTGlyphImpl.h 
    src/FTGlyph/FTGlyphGlue.cpp 
    src/FTGlyph/FTBitmapGlyph.cpp 
    src/FTGlyph/FTBitmapGlyphImpl.h 
    src/FTGlyph/FTBufferGlyph.cpp 
    src/FTGlyph/FTBufferGlyphImpl.h 
    src/FTGlyph/FTExtrudeGlyph.cpp 
    src/FTGlyph/FTExtrudeGlyphImpl.h 
    src/FTGlyph/FTOutlineGlyph.cpp 
    src/FTGlyph/FTOutlineGlyphImpl.h 
    src/FTGlyph/FTPixmapGlyph.cpp 
    src/FTGlyph/FTPixmapGlyphImpl.h 
    src/FTGlyph/FTPolygonGlyph.cpp 
    src/FTGlyph/FTPolygonGlyphImpl.h 
    src/FTGlyph/FTTextureGlyph.cpp 
    src/FTGlyph/FTTextureGlyphImpl.h 
	
    src/FTFont/FTFont.cpp 
    src/FTFont/FTFontImpl.h 
    src/FTFont/FTFontGlue.cpp 
    src/FTFont/FTBitmapFont.cpp 
    src/FTFont/FTBitmapFontImpl.h 
    src/FTFont/FTBufferFont.cpp 
    src/FTFont/FTBufferFontImpl.h 
    src/FTFont/FTExtrudeFont.cpp 
    src/FTFont/FTExtrudeFontImpl.h 
    src/FTFont/FTOutlineFont.cpp 
    src/FTFont/FTOutlineFontImpl.h 
    src/FTFont/FTPixmapFont.cpp 
    src/FTFont/FTPixmapFontImpl.h 
    src/FTFont/FTPolygonFont.cpp 
    src/FTFont/FTPolygonFontImpl.h 
    src/FTFont/FTTextureFont.cpp 
    src/FTFont/FTTextureFontImpl.h 
	
    src/FTLayout/FTLayout.cpp 
    src/FTLayout/FTLayoutImpl.h 
    src/FTLayout/FTLayoutGlue.cpp 
    src/FTLayout/FTSimpleLayout.cpp 
    src/FTLayout/FTSimpleLayoutImpl.h 
)
add_definitions(-DFTGL_LIBRARY_STATIC)
include_directories(msvc src ../freetype/include)
add_library(ftgl STATIC ${ftgl_SOURCES})
target_link_libraries( ftgl freetype)
SET_TARGET_PROPERTIES(ftgl PROPERTIES FOLDER 3rdparty)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(ftgl PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

