cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-stm32f4xx.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OPENOCD_EXECUTABLE C:/devtools/xpack-openocd-0.12.0-1/bin/openocd.exe)

project(firmware)
set(TARGET_ELF ${PROJECT_NAME}.elf)

add_definitions(-DSTM32F429xx)
add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-DHSE_VALUE=8000000)
add_definitions(-DSTM32F429_439xx)

add_executable(${TARGET_ELF} src/main.cpp)
enable_language(C CXX ASM)
target_compile_features(${TARGET_ELF} PRIVATE cxx_std_17)

add_subdirectory(system)
add_subdirectory(src)
add_subdirectory(libs/FreeRTOS)
add_subdirectory(libs/stm32tpl)
add_subdirectory(libs/TWI_Wrapper)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Linker/link.ld)
target_link_options(${TARGET_ELF} PUBLIC -T${LINKER_SCRIPT})
target_link_options(${TARGET_ELF} PUBLIC -Wl,-Map=$<TARGET_FILE:${TARGET_ELF}>.map)

target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_OBJDUMP} "-dx" $<TARGET_FILE:${TARGET_ELF}> > $<TARGET_FILE:${TARGET_ELF}>.lst
    COMMENT "Generating disassembler file")
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_OBJCOPY} "-Obinary" $<TARGET_FILE:${TARGET_ELF}> $<TARGET_FILE:${TARGET_ELF}>.bin
    COMMENT "Generating bin file")
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_OBJCOPY} "-Oihex" $<TARGET_FILE:${TARGET_ELF}> $<TARGET_FILE:${TARGET_ELF}>.hex
    COMMENT "Generating hex file")
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_SIZE} "-A" $<TARGET_FILE:${TARGET_ELF}>
    COMMENT "Output size of file")

add_custom_target(flash
        COMMAND ${OPENOCD_EXECUTABLE} -f board/stm32f429disc1.cfg
            -c init
            -c halt
            -c "program ${TARGET_ELF} verify"
            -c reset
            -c exit
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${tgt}
        COMMENT "Running openocd ${tgt_name}"
        USES_TERMINAL)
