if(WIN32)
	set(EXTLIB_NAME fbxsdk_win)
	set(EXTLIB_TGZ ${CMAKE_SOURCE_DIR}/external/${EXTLIB_NAME}.tar.gz)
	set(EXTLIB_DIR ${CMAKE_BINARY_DIR}/${EXTLIB_NAME})

	if(NOT EXISTS ${EXTLIB_DIR})
	  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf
		${EXTLIB_TGZ} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	endif(NOT EXISTS ${EXTLIB_DIR})

	set(FBX_DEFAULT_ROOT ${CMAKE_BINARY_DIR}/${EXTLIB_NAME})
elseif(UNIX)
endif()

# Setup the Fbx Sdk
set(FBX_ROOT ${FBX_DEFAULT_ROOT} CACHE PATH "The Fbx SDK Root Directory")

set(FBX_INCLUDE_DIR ${FBX_ROOT}/include)
include_directories(${FBX_INCLUDE_DIR})

if(APPLE)
    set(FBX_LIBDIR ${FBX_ROOT}/lib/gcc4/ub)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBDIR ${FBX_ROOT}/lib/gcc4)
elseif(MSVC80)
    SET(FBX_LIBDIR ${FBX_ROOT}/lib/vs2005)
elseif(MSVC90)
    SET(FBX_LIBDIR ${FBX_ROOT}/lib/vs2008)
elseif(MSVC10 OR MSVC_VERSION>1600)
    SET(FBX_LIBDIR ${FBX_ROOT}/lib/vs2010)
endif()

IF(APPLE)
    # do nothing
elseif(CMAKE_CL_64)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
elseif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
else()
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x86)
endif()

IF(APPLE)
    SET(FBX_LIBNAME ${FBX_LIBDIR}/libfbxsdk-2012.2-static.a)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBNAME ${FBX_LIBDIR}/libfbxsdk-2012.2-static.a)
ELSE()
	set(FBX_LIBNAME debug ${FBX_LIBDIR}/fbxsdk-2012.2-mdd.lib optimized ${FBX_LIBDIR}/fbxsdk-2012.2-mdd.lib)
endif()
