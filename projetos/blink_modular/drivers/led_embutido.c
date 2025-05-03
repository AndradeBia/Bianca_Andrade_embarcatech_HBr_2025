#include "led_embutido.h"
#include "pico/cyw43_arch.h"

int led_embutido_init(void) {
    if (cyw43_arch_init()) {
        return -1;
    }
    return 0;
}

void led_embutido_set(bool state) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
}