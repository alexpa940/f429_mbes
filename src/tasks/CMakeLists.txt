target_sources(${TARGET_ELF} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/hcsr04.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wlvgl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imu.cpp
)

target_include_directories(${TARGET_ELF} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

