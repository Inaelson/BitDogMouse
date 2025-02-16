#include "mouseFunction.h"
#include "wifi_config.h"

#define DEBOUNCE_TIME 200 // Tempo de debounce em ms


// Estruturas de timer para cada botão
struct repeating_timer debounce_timer_a;
struct repeating_timer debounce_timer_b;
struct repeating_timer debounce_timer_joystick;


/** Inicializa os pinos correspondetes aos botões A e B e o Joystick **/
void setup_buttons_function()
{
    gpio_init(BUTTON_A_PIN); // Inicializa o pino do botão A
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN); // Define o pino do botão A como entrada
    gpio_pull_up(BUTTON_A_PIN); // Habilita o pull-up no pino do botão A

    gpio_init(BUTTON_B_PIN); // Inicializa o pino do botão B
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN); // Define o pino do botão B como entrada
    gpio_pull_up(BUTTON_B_PIN); // Habilita o pull-up no pino do botão B

    gpio_init(JOYSTICK_BUTTON_PIN); // Inicializa o pino do botão do Joystick
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN); // Define o pino do botão do Joystick como entrada
    gpio_pull_up(JOYSTICK_BUTTON_PIN); // Habilita o pull-up no pino do botão do Joystick

    printf("Mouse function setup complete\n");
}

/** Função de callback para o debounce e reativação da interrupção do botão A **/
bool debounce_callback_a(struct repeating_timer *t)
{
    gpio_set_irq_enabled(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true); // Reativa a interrupção
    return false; // Retorna falso para não repetir a chamada

}

/** Função de callback para o debounce e reativação da interrupção do botão B **/
bool debounce_callback_b(struct repeating_timer *t)
{
    gpio_set_irq_enabled(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true); // Reativa a interrupção
    return false; // Retorna falso para não repetir a chamada
}

/** Função de callback para o debounce e reativação da interrupção do botão do Joystick **/
bool debounce_callback_joystick(struct repeating_timer *t)
{
    gpio_set_irq_enabled(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true); // Reativa a interrupção
    return false; // Retorna falso para não repetir a chamada
}

/** Função de callback para as interrupções **/
void mouse_irq_handler(uint gpio, uint32_t events)
{
    //Desativa a interrupção para evitar múltiplas chamadas
    gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL, false);

    // Verifica qual botão foi pressionado e ativa o timer de debounce correspondente
    if (gpio == BUTTON_A_PIN) {
        add_repeating_timer_ms(DEBOUNCE_TIME, debounce_callback_a, NULL, &debounce_timer_a);
        printf("Button A pressed\n");
    } else if (gpio == BUTTON_B_PIN) {
        add_repeating_timer_ms(DEBOUNCE_TIME, debounce_callback_b, NULL, &debounce_timer_b);
        printf("Button B pressed\n");
    } else if (gpio == JOYSTICK_BUTTON_PIN) {
        add_repeating_timer_ms(DEBOUNCE_TIME, debounce_callback_joystick, NULL, &debounce_timer_joystick);
        printf("Joystick button pressed\n");
    }
}
