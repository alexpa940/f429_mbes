
target_sources(${TARGET_ELF} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f429i_discovery.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tft/ili9341.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tft/tft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/touchpad/touchpad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/touchpad/stmpe811.c
)

target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tft)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/touchpad)
target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})