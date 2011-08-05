# Copyright (c) 2010 Daniel Pfeifer <daniel@pfeifer-mail.de>

find_package(Threads REQUIRED)

if(CMAKE_USE_PTHREADS_INIT)
  return()
endif(CMAKE_USE_PTHREADS_INIT)

IF(NOT WIN32)
  message(FATAL_ERROR "Equalizer requires Pthreads.")
ENDIF(NOT WIN32)

message(STATUS "Pthreads will be built by Equalizer.")

set(PTHREADS_NAME pthreads-w32-2-8-0-release)
set(PTHREADS_TGZ ${Equalizer_SOURCE_DIR}/externals/${PTHREADS_NAME}.tar.gz)
set(PTHREADS_DIR ${Equalizer_BINARY_DIR}/${PTHREADS_NAME})

if(NOT EXISTS ${PTHREADS_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf
    ${PTHREADS_TGZ} WORKING_DIRECTORY ${Equalizer_BINARY_DIR})
endif(NOT EXISTS ${PTHREADS_DIR})

file(COPY
  ${PTHREADS_DIR}/pthread.h
  ${PTHREADS_DIR}/sched.h
  DESTINATION ${Equalizer_BINARY_DIR}/include
  )

add_library(pthread SHARED
  ${PTHREADS_DIR}/attr.c
  ${PTHREADS_DIR}/barrier.c
  ${PTHREADS_DIR}/cancel.c
  ${PTHREADS_DIR}/cleanup.c
  ${PTHREADS_DIR}/condvar.c
  ${PTHREADS_DIR}/create.c
  ${PTHREADS_DIR}/dll.c
  ${PTHREADS_DIR}/exit.c
  ${PTHREADS_DIR}/fork.c
  ${PTHREADS_DIR}/global.c
  ${PTHREADS_DIR}/misc.c
  ${PTHREADS_DIR}/mutex.c
  ${PTHREADS_DIR}/private.c
  ${PTHREADS_DIR}/rwlock.c
  ${PTHREADS_DIR}/sched.c
  ${PTHREADS_DIR}/semaphore.c
  ${PTHREADS_DIR}/spin.c
  ${PTHREADS_DIR}/sync.c
  ${PTHREADS_DIR}/tsd.c
  ${PTHREADS_DIR}/nonportable.c
  )

SET_TARGET_PROPERTIES(pthread PROPERTIES FOLDER 3rdparty)
set_target_properties(pthread PROPERTIES COMPILE_DEFINITIONS PTW32_BUILD=1)
target_link_libraries(pthread Ws2_32)

install(TARGETS pthread
  ARCHIVE DESTINATION lib COMPONENT dev
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )
