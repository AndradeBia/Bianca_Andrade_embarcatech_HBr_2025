#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Inicializa ADC e GPIOs
void setup_adc() {
    adc_init();
    adc_gpio_init(26); // Canal 0
    adc_gpio_init(27); // Canal 1
}

// Lê valor do canal ADC especificado
uint16_t ler_adc(uint8_t canal) {
    adc_select_input(canal);
    return adc_read();
}

int main() {
    stdio_init_all();
    setup_adc();

    while (true) {
        uint16_t eixo_y = ler_adc(0);
        uint16_t eixo_x = ler_adc(1);

        printf("\nX: %d | Y: %d", eixo_x, eixo_y);
        sleep_ms(100);
    }
}
