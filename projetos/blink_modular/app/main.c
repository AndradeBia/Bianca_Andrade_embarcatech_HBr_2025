#include "pico/stdlib.h"
#include "hal_led.h"

#define BLINK_DELAY_MS 50

int main() {
    stdio_init_all();

    if (hal_led_init()) {
        return -1;
    }

    while (true) {
        hal_led_toggle();
        sleep_ms(BLINK_DELAY_MS);
    }

    return 0;
}