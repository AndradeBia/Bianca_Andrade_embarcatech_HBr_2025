cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(monitor_temperatura C CXX ASM)

pico_sdk_init()

add_executable(monitor_temperatura 
    monitor_temperatura.c
)

pico_set_program_name(monitor_temperatura "monitor_temperatura")
pico_set_program_version(monitor_temperatura "0.1")

pico_enable_stdio_uart(monitor_temperatura 0)
pico_enable_stdio_usb(monitor_temperatura 1)


target_link_libraries(monitor_temperatura
    pico_stdlib
    hardware_adc
)

target_include_directories(monitor_temperatura PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(monitor_temperatura)
