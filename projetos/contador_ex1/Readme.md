# Contador Decrescente com Display OLED e Botões para Raspberry Pi Pico

Este projeto implementa um contador regressivo simples (9 a 0) para a placa Raspberry Pi Pico (ou Pico W), utilizando um display OLED SSD1306 para visualização e dois botões para controle.

## Visão Geral

O programa utiliza interrupções de GPIO para detectar pressionamentos de botão e um timer periódico para controlar a contagem regressiva. O estado atual do contador e o número de vezes que o botão secundário foi pressionado durante a contagem são exibidos em um display OLED conectado via I2C.

## Requisitos de Hardware

*   Raspberry Pi Pico ou Pico W
*   Display OLED SSD1306 (Interface I2C)
*   2x Botões Táteis (Push Buttons)
*   Breadboard (Protoboard)
*   Fios Jumper

*Nota: O código utiliza os resistores pull-up internos do Pico para os botões, então resistores externos não são necessários.*

## Requisitos de Software

*   [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) (v1.5.1 ou compatível)
*   [CMake](https://cmake.org/) (v3.13 ou superior)
*   [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
*   (Opcional) Um IDE como Visual Studio Code com a extensão Pico-W-Go ou similar.

## Funcionalidade Detalhada

1.  **Inicialização:** Ao ligar, o display é inicializado e limpo. O sistema aguarda o primeiro pressionamento do Botão A.
2.  **Pressionar Botão A (GPIO 5):**
    *   O contador principal é **reiniciado para 9**.
    *   O contador de cliques do Botão B é **zerado**.
    *   O display é atualizado para mostrar "Tempo: 9" e "Acoes B: 0".
    *   A **contagem regressiva ativa** é iniciada. Um timer interno decrementa o contador principal a cada 1 segundo.
    *   As interrupções do Botão B são habilitadas (ou reabilitadas).
3.  **Durante a Contagem Regressiva (9 a 1):**
    *   A cada segundo, o contador principal diminui e o display é atualizado ("Tempo: X").
    *   Se o **Botão B (GPIO 6)** for pressionado durante este período:
        *   O contador de cliques do Botão B é incrementado.
        *   O display é atualizado para mostrar o novo valor de "Acoes B: Y".
4.  **Fim da Contagem:**
    *   Quando o contador principal atinge **0**:
        *   A contagem regressiva para.
        *   O display é atualizado para mostrar "Tempo: 0" e o valor final de "Acoes B: Y".
        *   As interrupções do Botão B são **desabilitadas**. Pressionar o Botão B neste estado **não** terá efeito no contador de cliques.
5.  **Estado Parado:**
    *   O sistema permanece neste estado, exibindo os valores finais (Tempo=0, Acoes B=Y), até que o Botão A seja pressionado novamente.
6.  **Reinício:**
    *   Pressionar o Botão A novamente reinicia todo o ciclo a partir do Passo 2.

## Pinagem (Conexões)

Baseado nos `#define`s no código (`contador_ex1.c`):

*   **Display OLED (I2C - i2c1):**
    *   `SDA` -> `GPIO 14` (Pino 19 do Pico)
    *   `SCL` -> `GPIO 15` (Pino 20 do Pico)
    *   `VCC` -> `3V3(OUT)` (Pino 36 do Pico)
    *   `GND` -> `GND` (Qualquer pino GND do Pico)
*   **Botão A:**
    *   Um terminal -> `GPIO 5` (Pino 7 do Pico)
    *   Outro terminal -> `GND` (Qualquer pino GND do Pico)
*   **Botão B:**
    *   Um terminal -> `GPIO 6` (Pino 9 do Pico)
    *   Outro terminal -> `GND` (Qualquer pino GND do Pico)

