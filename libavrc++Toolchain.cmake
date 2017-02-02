set(CMAKE_SYSTEM_NAME Generic)

find_program(CMAKE_C_COMPILER   "avr-gcc")
find_program(CMAKE_CXX_COMPILER "avr-g++")

set(LIBAVRC++_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Root dir of libavrc++")
mark_as_advanced(LIBAVRC++_DIR)

# Add current directory to CMake Module path automatically
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/Platform/libavrc++.cmake)
  set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})
  include(${CMAKE_CURRENT_LIST_DIR}/Platform/libavrc++.cmake)
endif()

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
