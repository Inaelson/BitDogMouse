#include <stdio.h>
#include "pico/stdlib.h"
#include "src/mouseFunction.h"



int main()
{
    stdio_init_all();
    sleep_ms(2000);
    printf("Monitor serial initialized\n");

    setup_mouse_function(); // Inicializa os pinos para utilização dos botões e do Joystick
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão A
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão B
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão do Joystick

    while(true) {
        tight_loop_contents();
    }
}
