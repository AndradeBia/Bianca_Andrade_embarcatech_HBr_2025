#include "hal_led.h"
#include "led_embutido.h"
#include <stdbool.h>

static bool led_state = false;

int hal_led_init(void) {
    return led_embutido_init();
}

void hal_led_toggle(void) {
    led_state = !led_state;
    led_embutido_set(led_state);
}