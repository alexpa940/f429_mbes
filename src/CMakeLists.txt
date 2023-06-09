
add_subdirectory(tasks)

target_sources(${TARGET_ELF} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f429.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOSHooks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx_hal_msp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/main_page.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/lv_analogclock.c

    ${CMAKE_CURRENT_SOURCE_DIR}/gui/lv_analogclock.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/gui/resources/Resources.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/resources/bitmaps/compass.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/resources/bitmaps/compass_earth.c

    )

target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gui)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gui/resources)

