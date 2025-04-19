# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

# Define project name here (used for executable name)
set(PROJECT_NAME joystick_leitura)
# Define C source file name here
set(SOURCE_FILE_NAME joystick_leitura.c)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location

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
# ====================================================================================

# Define o tipo de placa. 'pico' ou 'pico_w'.
set(PICO_BOARD pico CACHE STRING "Board type")

# Inclui o SDK do Raspberry Pi Pico (deve vir antes do 'project')
include(pico_sdk_import.cmake)

project(${PROJECT_NAME} C CXX ASM)

# Inicializa o SDK do Raspberry Pi Pico
pico_sdk_init()

# Adiciona o executável. O nome padrão é o nome do projeto.
add_executable(${PROJECT_NAME} ${SOURCE_FILE_NAME})

# Define metadados para o arquivo UF2
pico_set_program_name(${PROJECT_NAME} "${PROJECT_NAME}")
pico_set_program_version(${PROJECT_NAME} "0.1")

# Habilita stdio via USB (desabilita via UART)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Vincula as bibliotecas necessárias
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_adc
)

# Gera arquivos UF2, ELF, etc.
pico_add_extra_outputs(${PROJECT_NAME})
