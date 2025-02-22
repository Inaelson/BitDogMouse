#include "mouseFunction.h"
#include "wifi_config.h"

#define DEBOUNCE_TIME 200 // Tempo de debounce em ms

// Estruturas de timer para cada botão
struct repeating_timer debounce_timer_a;
struct repeating_timer debounce_timer_b;
struct repeating_timer debounce_timer_joystick;

/** Função de callback para o alarme de Wake-on-LAN **/
static int64_t wake_up_alarm(alarm_id_t id, void *user_data)
{
    gpio_put(LED_PIN_BLUE, 0); // Desliga o LED azul
    gpio_put(LED_PIN_GREEN, 1); // Liga o LED verde
    return 0;
}


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

void init_irq_buttons()
{
        /** Habilita as interrupções para os botões **/
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão A
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão B
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &mouse_irq_handler); // Habilita a interrupção para o botão do Joystick
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
        if(current_mode == MODE_MOUSE)
        {
            printf("Button A pressed\n"); // Mensagem de botão A pressionado
            send_udp_packet("ButtonA", 7); // Envia o pacote UDP
        }
        else if(current_mode == MODE_WOL)
        {
            gpio_put(LED_PIN_BLUE, 1); // Liga o LED azul
            gpio_put(LED_PIN_RED, 0); // Desliga o LED vermelho
            gpio_put(LED_PIN_GREEN, 0); // Desliga o LED verde

            printf("Sending Magic Packet\n");
            send_magic_packet(); // Envia o pacote Wake-on-LAN

            add_alarm_in_ms(2000, wake_up_alarm, NULL, false); // Adiciona um alarme para desligar o LED azul
        }
    } else if (gpio == BUTTON_B_PIN) {
        add_repeating_timer_ms(DEBOUNCE_TIME, debounce_callback_b, NULL, &debounce_timer_b);
        printf("Button B pressed\n"); // Mensagem de botão B pressionado
        send_udp_packet("ButtonB", 7); // Envia o pacote UDP
    } else if (gpio == JOYSTICK_BUTTON_PIN) {
        add_repeating_timer_ms(DEBOUNCE_TIME, debounce_callback_joystick, NULL, &debounce_timer_joystick);
        printf("Joystick button pressed\n"); // Mensagem de botão do Joystick pressionado
        send_udp_packet("ButtonJ", 7); // Envia o pacote UDP
    }
}
