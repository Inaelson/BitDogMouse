#include "src/mouseFunction.h"



int main()
{
    stdio_init_all();
    sleep_ms(2000); // Aguarda 2 segundos para inicialização do monitor serial
    printf("Monitor serial initialized\n");

    /** Funções de inicialização dos GPIO's **/
    setup_buttons_function(); // Inicializa os pinos dos botões
    setup_joystick_function(); // Inicializa os eixos X e Y do Joystick para leitura analógica


    /** Habilita as interrupções para os botões **/
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão A
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão B
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão do Joystick

    printf("Entrando no loop principal\n");
    while (true) {
        sleep_ms(1000); // Aguarda 1 segundo
        printf("valores do buffer: %d, %d\n", adc_buffer[0], adc_buffer[1]); // Imprime os valores do buffer
        while (pio_sm_is_rx_fifo_empty(pio, sm)) {
            sleep_us(10);  // Aguarda até que o FIFO tenha valores
        }

        uint32_t raw_x = pio_sm_get_blocking(pio, sm);
        uint16_t adc_x = raw_x & 0x0FFF;  // Extrai os 12 bits úteis de X

        while (pio_sm_is_rx_fifo_empty(pio, sm)) {
            sleep_us(10);  // Aguarda até que o FIFO tenha valores
        }

        uint32_t raw_y = pio_sm_get_blocking(pio, sm);
        uint16_t adc_y = raw_y & 0x0FFF;  // Extrai os 12 bits úteis de Y

        printf("Valor lido do FIFO - X: %d, Y: %d\n", adc_x, adc_y);

    }
}
