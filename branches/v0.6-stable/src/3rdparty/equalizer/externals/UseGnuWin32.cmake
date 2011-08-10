# Copyright (c) 2010 Daniel Pfeifer <daniel@pfeifer-mail.de>

if(NOT CMAKE_HOST_WIN32)
  return()
endif(NOT CMAKE_HOST_WIN32)

set(GNUWIN32_NAME GnuWin32)
set(GNUWIN32_TGZ ${Equalizer_SOURCE_DIR}/externals/${GNUWIN32_NAME}.tar.gz)
set(GNUWIN32_DIR ${Equalizer_BINARY_DIR}/${GNUWIN32_NAME})

if(NOT EXISTS ${GNUWIN32_DIR})
  file(MAKE_DIRECTORY ${GNUWIN32_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf
    ${GNUWIN32_TGZ} WORKING_DIRECTORY ${Equalizer_BINARY_DIR})
endif(NOT EXISTS ${GNUWIN32_DIR})

file(WRITE ${Equalizer_BINARY_DIR}/unistd.h.in "\n")
configure_file(${Equalizer_BINARY_DIR}/unistd.h.in
  ${Equalizer_BINARY_DIR}/include/unistd.h COPYONLY)

set(BISON_EXECUTABLE ${GNUWIN32_DIR}/bin/bison.bat)
set(FLEX_EXECUTABLE ${GNUWIN32_DIR}/bin/flex.exe)
