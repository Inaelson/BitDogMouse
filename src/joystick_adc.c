#include "mouseFunction.h"
#include "wifi_config.h"

char data[5]; // Buffer para armazenar os dados do ADC

/* Definições dos limites da zona neutra e ADC */
#define NEUTRAL_MIN 1800 // Valor mínimo para a zona neutra
#define NEUTRAL_MAX 2100 // Valor máximo para a zona neutra
#define ADC_MIN 0 // Valor mínimo do ADC
#define ADC_MAX 4095 // Valor máximo do ADC
#define SENSITIVITY_MIN -10 // Sensibilidade mínima
#define SENSITIVITY_MAX 10 // Sensibilidade máxima

/* Armazena os últimos valores enviados */
int last_x = 0;
int last_y = 0;
bool last_moving = false; // Indica se o joystick estava em movimento na última leitura

/* Protótipos das funções */
void setup_joystick_function();
bool read_joystick_adc(struct repeating_timer *t);
void send_packet(uint8_t *data, size_t len);
int adjust_sensibility(uint16_t valor);

/* Função de inicialização do Joystick */
void setup_joystick_function() {
    adc_init(); // Inicializa o ADC
    adc_gpio_init(JOYSTICK_Y_PIN);
    adc_gpio_init(JOYSTICK_X_PIN);

    printf("Joystick setup complete\n");
}

/* Função para ajustar a sensibilidade antes do envio */
int adjust_sensibility(uint16_t valor) {
    if (valor >= NEUTRAL_MIN && valor <= NEUTRAL_MAX) {
        return 0; // Zona neutra, sem movimentação
    }
    else if (valor < NEUTRAL_MIN) { // Movimento para esquerda/cima
        float scale = 1.0 - ((float)(valor - ADC_MIN) / (NEUTRAL_MIN - ADC_MIN));
        return (int)(SENSITIVITY_MIN * scale);
    }
    else { // Movimento para direita/baixo (sem inversão da scale)
        float scale = (float)(valor - NEUTRAL_MAX) / (ADC_MAX - NEUTRAL_MAX);
        return (int)(SENSITIVITY_MAX * scale);
    }
}

/* Função de callback para o timer de leitura do ADC */
bool read_joystick_adc(struct repeating_timer *t) {
    adc_select_input(0); // Seleciona o pino do eixo X
    uint16_t y_raw = adc_read(); // Realiza a leitura do ADC
    adc_select_input(1); // Seleciona o pino do eixo Y
    uint16_t x_raw = adc_read(); // Realiza a leitura do ADC

    // Ajusta a sensibilidade antes de enviar
    int x_adjusted = adjust_sensibility(x_raw);
    int y_adjusted = adjust_sensibility(y_raw);

    // Determina se o joystick está em movimento
    bool moving = (x_adjusted != 0 || y_adjusted != 0);

    // Enviar apenas se houver mudança no estado do joystick (movimento ou parada)
    if (x_adjusted != last_x || y_adjusted != last_y || moving != last_moving) {
        if (moving) {
            sprintf(data, "%d,%d", x_adjusted, y_adjusted);
        } else {
            sprintf(data, "STOP");
        }
        send_packet(data, strlen(data));

        // Atualiza os últimos valores
        last_x = x_adjusted;
        last_y = y_adjusted;
        last_moving = moving;
    }

    return true;
}

/* Função para envio de pacotes através da rede */
void send_packet(uint8_t *data, size_t len) {
    send_udp_packet(data, len);
}
