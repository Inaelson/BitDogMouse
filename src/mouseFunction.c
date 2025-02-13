#include <stdio.h>
#include "pico/stdlib.h"
#include "mouseFunction.h"
#include "hardware/adc.h"
#include "hardware/irq.h"
#include "hardware/timer.h"

#define DEBOUNCE_TIME 200 // Tempo de debounce em ms


/** Estrutura de dados para debounce **/
typedef struct{
    uint8_t gpio;
    struct repeating_timer debounce_timer;
} debounce_data_t;

/** Vetor de estruturas de dados para debounce **/
debounce_data_t debounce_data[3] = {
    {BUTTON_A_PIN},
    {BUTTON_B_PIN},
    {JOYSTICK_BUTTON_PIN}
};


/** Inicializa os pinos correspondetes aos botões A e B e o Joystick **/
void setup_mouse_function()
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

    adc_init(); // Inicializa o ADC
    adc_gpio_init(JOYSTICK_X_PIN); // Inicializa o pino do eixo X do Joystick
    adc_gpio_init(JOYSTICK_Y_PIN); // Inicializa o pino do eixo Y do Joystick

    printf("Mouse function setup complete\n");
}

bool debounce_callback(struct repeating_timer *t)
{
    debounce_data_t *data = (debounce_data_t *)t->user_data; // Converte o ponteiro para a estrutura de dados de debounce
    gpio_set_irq_enabled(data->gpio, GPIO_IRQ_EDGE_FALL, true); // Reativa a interrupção
    return false; // Retorna falso para não repetir a chamada

}

/** Função de callback para as interrupções **/
void mouse_irq_handler(uint8_t gpio, uint32_t events)
{
    //Encontra a estrutura correspondente ao pino que gerou a interrupção
    debounce_data_t *data = NULL;
    for(int i = 0; i < 3; i++) {
        if(debounce_data[i].gpio == gpio) {
            data = &debounce_data[i];
            break;
        }
    }

    if(!data) return; // Retorna se não encontrar a estrutura

    //Desativa a interrupção para evitar múltiplas chamadas
    gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL, false);

    //Reativa a interrupção após 20ms
    static struct repeating_timer debounce_timer;
    add_repeating_timer_ms(DEBOUNCE_TIME, debounce_callback, (void *)data, &debounce_timer);

    if (gpio == BUTTON_A_PIN) {
        printf("Button A pressed\n");
    } else if (gpio == BUTTON_B_PIN) {
        printf("Button B pressed\n");
    } else if (gpio == JOYSTICK_BUTTON_PIN) {
        printf("Joystick button pressed\n");
    }
}
