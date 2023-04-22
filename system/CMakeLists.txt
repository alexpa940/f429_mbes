
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f4-hal STMHAL_SOURCES)

target_sources(${TARGET_ELF} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cmsis/system_stm32f4xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/newlib/cxx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/newlib/new.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/newlib/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/newlib/exit.c
    ${STMHAL_SOURCES}
)


target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/cmsis)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stm32f4-hal)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stm32f4-hal/Legacy)