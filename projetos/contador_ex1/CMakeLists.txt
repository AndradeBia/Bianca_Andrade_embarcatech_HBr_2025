# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17) # Mantido por padrão, mas não estritamente necessário se só usar C
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Útil para ferramentas de análise

# --- Configuração VS Code Extension (NÃO EDITAR) ---
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
# As versões abaixo são exemplos, não precisam ser alteradas a menos que saiba o que está fazendo
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# --- Fim Configuração VS Code Extension ---

# Define a placa alvo (ajuste se necessário)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o SDK do Pico (deve vir antes do 'project')
include(pico_sdk_import.cmake)

# Define o nome do projeto
project(contador_ex1 C CXX ASM) # Nome do projeto (pode ser diferente do executável)

# Inicializa o SDK do Pico
pico_sdk_init()

# --- Definição do Executável ---
# Adiciona o executável e seus arquivos fonte
# Use o nome do seu arquivo .c principal aqui
add_executable(contador_ex1
    contador_ex1.c          # Seu arquivo C principal
    inc/ssd1306_i2c.c       # Arquivo de implementação do driver SSD1306
)

# Define o nome do programa (geralmente o mesmo nome do executável)
pico_set_program_name(contador_ex1 "contador_oled")
pico_set_program_version(contador_ex1 "0.1")

# --- Configuração STDIO ---
# Habilita/desabilita saída via UART ou USB
pico_enable_stdio_uart(contador_ex1 0) # 0 = desabilitado
pico_enable_stdio_usb(contador_ex1 1)  # 1 = habilitado

# --- Bibliotecas Necessárias ---
# Adiciona diretórios de include (necessário para encontrar "inc/ssd1306.h")
target_include_directories(contador_ex1 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR} # Diretório atual onde está o CMakeLists.txt
)

# Linka as bibliotecas do SDK necessárias
target_link_libraries(contador_ex1 PRIVATE # Use PRIVATE para encapsular
    pico_stdlib        # Biblioteca padrão (inclui GPIO, Utils, etc.)
    hardware_i2c       # Para comunicação I2C com o display
    hardware_timer     # Para o timer periódico (repeating_timer)
    # hardware_gpio já está incluso em pico_stdlib, mas pode ser explícito se preferir
)

# --- Saídas Adicionais ---
# Gera arquivos .uf2, .hex, .elf etc.
pico_add_extra_outputs(contador_ex1)