#=============================================================================#
macro(_libavrcpp_load_config config)
  if(NOT EXISTS ${LIBAVRC++_DIR}/configs/${config}.cmake)
    message(FATAL_ERROR "The config ${config} does not exists")
  endif()
  include(${LIBAVRC++_DIR}/configs/${config}.cmake)

  set(_board_name ${LIBAVRC++_NAME})
  if(DEFINED LIBAVRC++_VARIANTS)
    if(DEFINED _libavrc++_VARIANT)
      set(_board_name "${_board_name} - ${LIBAVRC++_${_libavrc++_VARIANT}}")
      get_directory_property(_variables VARIABLES)
      foreach(_variable ${_variables})
        if(_variable MATCHES "LIBAVRC\\+\\+_${_libavrc++_VARIANT}_(.*)")
          set(LIBAVRC++_${CMAKE_MATCH_1} ${${_variable}})
          unset(LIBAVRC++_${_variant})
        endif()
      endforeach()
    else()
      set(_msg "You must choose a variant:")
      foreach(_variant ${LIBAVRC++_VARIANTS})
        set(_msg "${_msg}\n - ${LIBAVRC++_${_variant}}")
      endforeach()
      message(FATAL_ERROR ${_msg})
    endif()
    unset(LIBAVRC++_VARIANTS)
  endif()
  message("Loaded configuration for board: ${_board_name}")
endmacro()

#=============================================================================#
function(libavrcpp_list_configs)
  file(GLOB _configs
    RELATIVE "${LIBAVRC++_DIR}/configs"
    "${LIBAVRC++_DIR}/configs/*.cmake"
    )

  message(STATUS "List of available configs:")
  foreach(_config ${_configs})
    string(REPLACE ".cmake" "" _config_name "${_config}")

    include(${LIBAVRC++_DIR}/configs/${_config_name}.cmake)

    message(STATUS "  ${_config_name}: ${LIBAVRC++_NAME}")
    if(DEFINED LIBAVRC++_VARIANTS)
      foreach(_variant ${LIBAVRC++_VARIANTS})
        message(STATUS "      ${_variant}: ${LIBAVRC++_${_variant}}")
        unset(LIBAVRC++_${_variant})
      endforeach()
      unset(LIBAVRC++_VARIANTS)
    endif()
  endforeach()
endfunction()

include(CMakeParseArguments)

#=============================================================================#
function(add_avr_firmware target)
  cmake_parse_arguments(_libavrc++ "" "PORT;CONFIG;VARIANT" "" ${ARGN})

  if(NOT _libavrc++_CONFIG)
    message(FATAL_ERROR "You should specify a CONFIG for libavr_add_firmware")
  endif()

  _libavrcpp_load_config(${_libavrc++_CONFIG})

  set(_libavrc++_srcs
    ${LIBAVRC++_DIR}/scripts/ldd_hack.cpp
    ${LIBAVRC++_DIR}/src/common/common.cc
    ${LIBAVRC++_DIR}/src/main.cc
    )

  if(_libavrc++_PORT)
    set(LIBAVRC++_PORT ${_libavrc++_PORT})
  else()
    set(LIBAVRC++_PORT $ENV{LIBAVRC++_PORT})
    if (NOT LIBAVRC++_PORT)
      set(LIBAVRC++_PORT ${LIBAVRC++_UPLOAD_DEFAULT_PORT})
    endif()
  endif()

  add_library(_${target} STATIC
    ${_libavrc++_UNPARSED_ARGUMENTS})

  get_target_property(_objs _${target} RESSOURCE)
  message("_${target} ${_objs}")

  add_custom_command(TARGET _${target} POST_BUILD
    COMMAND ${LIBAVRC++_DIR}/scripts/nm.sh ${AVR_NM} ${_name} lib_${target}.a ${CMAKE_BINARY_DIR}/_${target}_depends.cc ${LIBAVRC++_DIR}
    BYPRODUCTS ${CMAKE_BINARY_DIR}/_${target}_depends.cc
    )

  add_executable(${target} ${_libavrc++_srcs} ${CMAKE_BINARY_DIR}/_${target}_depends.cc)
  target_link_libraries(${target} PRIVATE _${target})


  set(_libavrc++_DEFINITIONS "-mmcu=${LIBAVRC++_BUILD_MCU} -DF_CPU=${LIBAVRC++_BUILD_F_CPU} -DMCU=${LIBAVRC++_BUILD_MCU}")

  if(LIBAVRC++_BUILD_VID)
    set(_libavrc++_DEFINITIONS "${_libavrc++_DEFINITIONS} -DUSB_VID=${LIBAVRC++_BUILD_VID} -DUSB_PID=${LIBAVRC++_BUILD_PID}")
  endif()

  include_directories(
    ${LIBAVRC++_DIR}/src
    ${LIBAVRC++_DIR}/src/comm)

  set_target_properties(${target} _${target}
    PROPERTIES COMPILE_FLAGS ${_libavrc++_DEFINITIONS}
    LINK_FLAGS "-mmcu=${LIBAVRC++_BUILD_MCU}"
    )
  set_target_properties(${target}
    PROPERTIES OUTPUT_NAME "${target}.elf")

  if(CMAKE_OBJCOPY AND AVRDUDE)
    if(AVR_SIZE)
      add_custom_target(${target}_size)
      add_dependencies(${target}_size ${target})

      add_custom_command(TARGET ${target}_size POST_BUILD
        COMMAND ${AVR_SIZE} -C --mcu=${LIBAVRC++_BUILD_MCU} --target=ihex ${target}.elf
        DEPENDS ${target}
        )
    endif()

    add_custom_target(${target}_hex)
    add_dependencies(${target}_hex ${target} ${target}_size)

    add_custom_command(TARGET ${target}_hex POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/${target}.elf ${target}.hex
      COMMENT "Generating flash memory file ${target}.hex from ${target}.elf"
      )

    add_custom_command(OUTPUT ${target}.eep
      COMMAND ${CMAKE_OBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ${CMAKE_CURRENT_BINARY_DIR}/${target}.elf ${target}.eep
      DEPENDS ${target}
      COMMENT "Generating eeprom memory file ${target}.hex from ${target}.elf"
      )

    if(LIBAVRC++_BOOTLOADER_PATH STREQUAL "caterina")
      set(_libavrc++_reset_opt "--caterina")
    else()
      unset(_libavrc++_reset_opt)
    endif()

    if(_libavrc++_reset_opt)
      set(_libavrc++_upload_depends ${target}_hex ${target}_reset)
      add_custom_target(${target}_reset
        COMMAND ${LIBAVRC++_DIR}/scripts/ard-reset-arduino ${_libavrc++_reset_opt} ${LIBAVRC++_PORT}
        COMMENT "Forcing reset of \"${LIBAVRC++_NAME} (${LIBAVRC++_BUILD_MCU})\" on port ${LIBAVRC++_PORT}"
        )
    else()
      set(_libavrc++_upload_depends ${target}_hex)
    endif()

    if("${LIBAVRC++_UPLOAD_PROTOCOL}" STREQUAL "usbtiny")
      add_custom_target(${target}_upload
        COMMAND ${AVRDUDE} -C${LIBAVRC++_DIR}/scripts/avrdude.conf -c${LIBAVRC++_UPLOAD_PROTOCOL} -p${LIBAVRC++_BUILD_MCU} -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/${target}.hex:i -v
        DEPENDS ${_libavrc++_upload_depends}
        COMMENT "Uploading ${target}.hex to \"${LIBAVRC++_NAME} (${LIBAVRC++_BUILD_MCU})\" using programmer ${LIBAVRC++_UPLOAD_PROTOCOL} on port ${LIBAVRC++_PORT}:${LIBAVRC++_UPLOAD_SPEED}"
        )
    else()
      add_custom_target(${target}_upload
        COMMAND ${AVRDUDE} -C${LIBAVRC++_DIR}/scripts/avrdude.conf -P${LIBAVRC++_PORT} -b${LIBAVRC++_UPLOAD_SPEED} -c${LIBAVRC++_UPLOAD_PROTOCOL} -p${LIBAVRC++_BUILD_MCU} -D -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/${target}.hex:i -q -q
        DEPENDS ${_libavrc++_upload_depends}
        COMMENT "Uploading ${target}.hex to \"${LIBAVRC++_NAME} (${LIBAVRC++_BUILD_MCU})\" using programmer ${LIBAVRC++_UPLOAD_PROTOCOL}"
        )
    endif()
  endif()
endfunction()


  #=============================================================================#
  find_program(AVR_SIZE       "avr-size")
  find_program(AVR_NM         "avr-nm")
  find_program(AVRDUDE        "avrdude")
  find_program(CMAKE_OBJCOPY  "avr-objcopy")

  mark_as_advanced(AVR_SIZE AVRDUDE)

  #=============================================================================#
  #                              C Flags
  #=============================================================================#
  set(LIBAVRC++_C_FLAGS "-mcall-prologues -ffunction-sections -fdata-sections")
  set(CMAKE_C_FLAGS                "-Os -Wall   ${LIBAVRC++_C_FLAGS}" CACHE STRING "")
  set(CMAKE_C_FLAGS_DEBUG          "-g          ${LIBAVRC++_C_FLAGS}" CACHE STRING "")
  set(CMAKE_C_FLAGS_MINSIZEREL     "-DNDEBUG    ${LIBAVRC++_C_FLAGS}" CACHE STRING "")
  set(CMAKE_C_FLAGS_RELEASE        "-DNDEBUG -w ${LIBAVRC++_C_FLAGS}" CACHE STRING "")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g       -w ${LIBAVRC++_C_FLAGS}" CACHE STRING "")

  #=============================================================================#
  #                             C++ Flags
  #=============================================================================#
  set(LIBAVRC++_CXX_FLAGS "${LIBAVRC++_C_FLAGS} -fno-exceptions")
  set(CMAKE_CXX_FLAGS                "-Os -Wall ${LIBAVRC++_CXX_FLAGS}" CACHE STRING "")
  set(CMAKE_CXX_FLAGS_DEBUG          "-g        ${LIBAVRC++_CXX_FLAGS}" CACHE STRING "")
  set(CMAKE_CXX_FLAGS_MINSIZEREL     "-DNDEBUG  ${LIBAVRC++_CXX_FLAGS}" CACHE STRING "")
  set(CMAKE_CXX_FLAGS_RELEASE        "-DNDEBUG  ${LIBAVRC++_CXX_FLAGS}" CACHE STRING "")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g        ${LIBAVRC++_CXX_FLAGS}" CACHE STRING "")

  #=============================================================================#
  #                       Executable Linker Flags                               #
  #=============================================================================#
  set(LIBAVRC++_LINKER_FLAGS "-Wl,--gc-sections")
  set(CMAKE_EXE_LINKER_FLAGS                "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG          "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL     "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE        "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")

  #=============================================================================#
  #=============================================================================#
  #                       Shared Lbrary Linker Flags                            #
  #=============================================================================#
  set(CMAKE_SHARED_LINKER_FLAGS                "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG          "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL     "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE        "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")

  set(CMAKE_MODULE_LINKER_FLAGS                "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_MODULE_LINKER_FLAGS_DEBUG          "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL     "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_MODULE_LINKER_FLAGS_RELEASE        "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")
  set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${LIBAVRC++_LINKER_FLAGS}" CACHE STRING "")

  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")