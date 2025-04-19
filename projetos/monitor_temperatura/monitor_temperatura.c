#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Inicializa e configura o sensor de temperatura interno
void inicializar_sensor_temp() {
    adc_init(); // Inicializa o ADC
    adc_set_temp_sensor_enabled(true); // Ativa o sensor de temperatura interno
    adc_select_input(4); // Seleciona o canal ADC 4 (sensor de temperatura)
}

// Converte leitura bruta do ADC em temperatura em Celsius
float ler_temperatura_celsius() {
    const float tensao_referencia = 3.3f;
    const float resolucao_adc = 1 << 12; // 12 bits = 4096
    const float fator_conversao = tensao_referencia / resolucao_adc;

    uint16_t leitura_bruta = adc_read(); 
    float tensao = leitura_bruta * fator_conversao; // Converte para tensão

    // Fórmula baseada no datasheet do RP2040
    float temperatura_c = 27.0f - (tensao - 0.706f) / 0.001721f;
    return temperatura_c;
}

int main(void) {
    stdio_init_all(); 
    inicializar_sensor_temp(); 

    while (true) {
        float temperatura = ler_temperatura_celsius(); // Lê temperatura
        printf("Temperatura = %.1f C\n", temperatura);
        sleep_ms(1000);
    }
}
