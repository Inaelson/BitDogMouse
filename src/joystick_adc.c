#include "mouseFunction.h"
#include "joystick_xy.pio.h"

// Macros de configuração do Joystick
#define SAMPLE_RATE 2000 // Taxa de amostragem em microsegundos de 500Hz (2ms) para o Joystick
#define DMA_CHANNEL 0 // Canal DMA para o Joystick
#define BUFFER_SIZE 2 // Tamanho do buffer para o Joystick

uint sm = 0; // Seleciona a State Machine 0
PIO pio = pio0; // Seleciona o PIO 0

struct repeating_timer timer_read_adc; // Cria uma estrutura de timer para leitura do ADC

uint16_t adc_buffer[BUFFER_SIZE]; // Buffer para armazenar os valores lidos do ADC

/* Protótipo das funções */
void setup_joystick_function();
bool read_joystick_adc(struct repeating_timer *t);
static inline void joystick_xy_program_init(PIO pio, uint sm, uint offset);
void setup_joystick_dma();

/* Função de inicialização do Joystick */
void setup_joystick_function(){
    adc_init(); // Inicializa o ADC
    adc_gpio_init(JOYSTICK_X_PIN); // Inicializa o pino do eixo X do Joystick no ADC0
    adc_gpio_init(JOYSTICK_Y_PIN); // Inicializa o pino do eixo Y do Joystick no ADC1

    uint offset = pio_add_program(pio, &joystick_xy_program); // Adiciona o programa ao PIO
    joystick_xy_program_init(pio, sm, offset); // Inicializa o programa do PIO

    setup_joystick_dma(); // Configura o DMA para o Joystick

    // Adiciona um timer para leitura do ADC
    bool success = add_repeating_timer_us(-SAMPLE_RATE, read_joystick_adc, NULL, &timer_read_adc);

    if (!success){
        printf("Erro ao adicionar o timer\n");
    } else {
        printf("Joystick setup complete\n");
    }
}

/* Função de callback para o timer de leitura do ADC */
bool read_joystick_adc(struct repeating_timer *t){
    adc_select_input(0); // Seleciona o pino do eixo X do Joystick
    uint16_t x = adc_read(); // Lê o valor do eixo X do Joystick
    pio_sm_put_blocking(pio, sm, x); // Envia o valor lido em X para o programa do PIO
    adc_select_input(1); // Seleciona o pino do eixo Y do Joystick
    uint16_t y = adc_read(); // Lê o valor do eixo Y do Joystick
    pio_sm_put_blocking(pio, sm, y); // Envia o valor lido em Y para o programa do PIO

    return true;
}


/* Função de configuração do DMA */
void setup_joystick_dma(){
    dma_channel_config c = dma_channel_get_default_config(DMA_CHANNEL); // Configuração padrão do canal DMA
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16); // Configura o tamanho da transferência para 16 bits
    channel_config_set_read_increment(&c, false); // Desabilita o incremento do endereço de leitura
    channel_config_set_write_increment(&c, true); // Habilita o incremento do endereço de escrita
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false)); // Configura o DREQ para o PIO

    dma_channel_configure(
        DMA_CHANNEL, // Canal DMA
        &c, // Configuração do canal DMA
        adc_buffer, // Endereço de escrita
        &pio->rxf[sm], // Endereço de leitura
        BUFFER_SIZE, // Tamanho da transferência
        true); // Habilita a transferência de hardware (automaticamente)

}