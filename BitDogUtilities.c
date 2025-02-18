#include "src/mouseFunction.h"
#include "src/wifi_config.h"



int main()
{
    stdio_init_all();
    sleep_ms(2000); // Aguarda 2 segundos para inicialização do monitor serial
    printf("Monitor serial initialized\n");

    gpio_init(LED_PIN_RED); // Inicializa o pino do LED vermelho
    gpio_init(LED_PIN_GREEN); // Inicializa o pino do LED verde
    gpio_set_dir(LED_PIN_RED, GPIO_OUT); // Define o pino do LED vermelho como saída
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT); // Define o pino do LED verde como saída
    gpio_put(LED_PIN_RED, 1); // Liga o LED vermelho
    gpio_put(LED_PIN_GREEN, 1); // Liga o LED verde

    /** Funções de inicialização dos GPIO's **/
    setup_buttons_function(); // Inicializa os pinos dos botões
    setup_joystick_function(); // Inicializa os eixos X e Y do Joystick para leitura analógica


    /** Habilita as interrupções para os botões **/
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão A
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão B
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão do Joystick

    wifi_init(); // Inicializa o Wi-Fi e configura a conexão UDP

    printf("Entrando no loop principal\n");
    while (true) {
        cyw43_arch_poll();
        sleep_ms(100);
    }
}
