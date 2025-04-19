# 📡 monitor_temperatura

Projeto para leitura da temperatura interna do microcontrolador **Raspberry Pi Pico W**, utilizando o ADC e exibindo os dados pela porta USB com `stdio_usb`.

## 🧠 Descrição

Este projeto lê a temperatura do sensor interno do chip RP2040, converte para Celsius utilizando a fórmula oficial do datasheet, e exibe o valor via saída padrão USB a cada segundo.

## 🛠️ Requisitos

- Raspberry Pi Pico ou Pico W
- [SDK do Raspberry Pi Pico](https://github.com/raspberrypi/pico-sdk)
- CMake ≥ 3.13
- Compilador `arm-none-eabi-gcc`
- Ninja (opcional, mas recomendado)
- VS Code com a extensão Pico (opcional)
