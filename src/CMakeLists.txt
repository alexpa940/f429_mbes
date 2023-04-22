
add_subdirectory(tasks)

target_sources(${TARGET_ELF} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f429.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOSHooks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx_hal_msp.cpp
)
