set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CROSSTOOL_PATH C:\\devtools\\xpack-arm-none-eabi-gcc-12.2.1-1.2\\bin CACHE PATH "Cross toolchain root directory")

function(find_toolchain)
  foreach(CROSS_PREFIX ${ARGV})
    find_program(CROSS_CC "${CROSS_PREFIX}gcc" "${CROSSTOOL_PATH}")
    find_program(CROSS_CXX "${CROSS_PREFIX}g++" "${CROSSTOOL_PATH}")
    find_program(CROSS_OBJDUMP "${CROSS_PREFIX}objdump" "${CROSSTOOL_PATH}")
    find_program(CROSS_OBJCOPY "${CROSS_PREFIX}objcopy" "${CROSSTOOL_PATH}")
    find_program(CROSS_SIZE "${CROSS_PREFIX}size" "${CROSSTOOL_PATH}")
    if (CROSS_CC AND CROSS_CXX AND CROSS_OBJCOPY AND CROSS_OBJCOPY AND CROSS_SIZE)
      message("@@ found toolchain ${CROSS_CC}")
      set(CMAKE_C_COMPILER ${CROSS_CC} PARENT_SCOPE)
      set(CMAKE_CXX_COMPILER ${CROSS_CXX} PARENT_SCOPE)
      return()
    endif()
  endforeach()
endfunction()

if(CMAKE_C_COMPILER)
  message("** use cmake toolchain")
  string(REGEX REPLACE "\-gcc$" "-objdump" CROSS_OBJDUMP "${CMAKE_C_COMPILER}")
  string(REGEX REPLACE "\-gcc$" "-objcopy" CROSS_OBJCOPY "${CMAKE_C_COMPILER}")
else()
  message("** find toolchain")
  find_toolchain("arm-none-eabi-")
endif()

set(CPU_FLAGS "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(STM_CXX_FLAGS "-fno-rtti -fno-exceptions -fno-use-cxa-atexit")
set(COMMON_FLAGS "-fdata-sections -ffunction-sections -fmessage-length=0 -fsigned-char -fno-common -Wunused -Wuninitialized")

set(FLAGS_DEBUG "-Og -g")
set(FLAGS_RELEASE "-O2")
set(FLAGS_SIZE "-Os")

set(CMAKE_C_FLAGS "${CPU_FLAGS} ${OPT_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG ${FLAGS_DEBUG})
set(CMAKE_C_FLAGS_RELEASE ${FLAGS_RELEASE})
set(CMAKE_C_FLAGS_MINSIZEREL ${FLAGS_SIZE})
set(CMAKE_CXX_FLAGS "${CPU_FLAGS} ${STM_CXX_FLAGS} ${OPT_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG ${FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_MINSIZEREL ${FLAGS_SIZE})
SET(CMAKE_ASM_FLAGS "${CFLAGS} ${CPU_FLAGS} -x assembler-with-cpp")

set(LD_FLAGS "--specs=nano.specs -u _printf_float -nostartfiles -Wl,--gc-sections -Wl,--print-memory-usage -Xlinker --wrap=malloc -Xlinker --wrap=_malloc_r")

set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS}  ${LD_FLAGS}" CACHE INTERNAL "") 

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
