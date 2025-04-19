# 📟 Leitura de Joystick com Raspberry Pi Pico

Este projeto realiza a leitura dos eixos X e Y de um joystick analógico usando o conversor ADC (Analog-to-Digital Converter) do Raspberry Pi Pico.

## 📁 Estrutura do Projeto

```
joystick_leitura/
├── CMakeLists.txt
├── joystick_leitura.c
└── README.md
```

## ⚙️ Requisitos

- Raspberry Pi Pico (ou Pico W)
- VS Code com a extensão oficial do Raspberry Pi Pico (ou ambiente de build CMake configurado)
- Joystick analógico (com saídas para X, Y e GND/VCC)
- Fios de conexão
- SDK do Raspberry Pi Pico (instalado previamente)

## 🧠 Funcionamento

O joystick é conectado aos pinos ADC do Pico:
- Eixo **X** no **GPIO27** (canal ADC1)
- Eixo **Y** no **GPIO26** (canal ADC0)

O código lê os valores analógicos de cada eixo e os imprime via USB com `printf`.

## 🔌 Conexões

| Joystick | Pico GPIO | Função |
|----------|-----------|--------|
| VRx      | GPIO27    | ADC1 (X) |
| VRy      | GPIO26    | ADC0 (Y) |
| GND      | GND       | Terra |
| VCC      | 3.3V      | Alimentação |


