cmake_minimum_required(VERSION 3.13)

set(UNITY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/unity/src)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(pico_temp_test C CXX ASM)

pico_sdk_init()


message(STATUS "Unit testing enabled (only target)")

add_executable(temp_sensor_test
    test/test_temp_sensor.c
    temp_sensor.c
    ${UNITY_SRC_DIR}/unity.c
)

target_include_directories(temp_sensor_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${UNITY_SRC_DIR}
)

target_link_libraries(temp_sensor_test PRIVATE
    pico_stdlib
)

pico_add_extra_outputs(temp_sensor_test)

message(STATUS "Test executable 'temp_sensor_test' configured as the only target.")