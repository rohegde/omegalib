# Copyright (c) 2010 Daniel Pfeifer <daniel@pfeifer-mail.de>
set(GLEW_NAME glew-1.5.7.3)
set(GLEW_TGZ ${Equalizer_SOURCE_DIR}/externals/${GLEW_NAME}.tgz)
set(GLEW_DIR ${Equalizer_BINARY_DIR}/${GLEW_NAME})

if(NOT EXISTS ${GLEW_DIR})
message(STATUS "  Extracting GLEW to ${GLEW_DIR}")
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf
  ${GLEW_TGZ} WORKING_DIRECTORY ${Equalizer_BINARY_DIR})
endif(NOT EXISTS ${GLEW_DIR})

set(GLEW_INCLUDE_DIRS ${GLEW_DIR}/include)
set(GLEW_LIBRARY ${GLEW_NAME}-MX-Equalizer)

add_definitions(-DGLEW_STATIC=1 -DGLEW_MX=1)
add_library(${GLEW_LIBRARY} STATIC ${GLEW_DIR}/src/glew.c)
SET_TARGET_PROPERTIES(${GLEW_LIBRARY} PROPERTIES FOLDER 3rdparty)

if(EQ_GLX_USED)
add_definitions(-DGLEW_APPLE_GLX=1)
endif(EQ_GLX_USED)

install(DIRECTORY ${GLEW_INCLUDE_DIRS} DESTINATION . COMPONENT dev)
install(TARGETS ${GLEW_LIBRARY} ARCHIVE DESTINATION lib COMPONENT dev)

include_directories(BEFORE SYSTEM ${GLEW_INCLUDE_DIRS})
