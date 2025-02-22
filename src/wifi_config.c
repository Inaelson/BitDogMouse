#include "wifi_config.h"
#include "mouseFunction.h"

struct udp_pcb *udp_conn; // Conexão UDP
struct repeating_timer timer_read_adc; // Timer para leitura do ADC

volatile int current_mode = MODE_WOL; // Inicia no modo Wake-on-LAN


// Função de callback para recepção de pacotes
static void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    if (p != NULL) {
        printf("Pacote recebido! Tamanho: %d\n", p->tot_len);
        current_mode = MODE_MOUSE; // Altera o modo para mouse

        // Ativa um timer para leitura do ADC
        add_repeating_timer_us(-SAMPLE_RATE, read_joystick_adc, NULL, &timer_read_adc);

        pbuf_free(p);
    }
}

/* Função para reconexão do Wi-Fi em caso de falha */
void wifi_reconnect() {
    cyw43_arch_deinit(); // Desconecta do Wi-Fi
    sleep_ms(2000); // Aguarda 1 segundo
    gpio_put(LED_PIN_GREEN, 1); // Liga novamente o LED verde
    wifi_init(); // Realiza uma nova tentativa de conexão
}

// Inicializa Wi-Fi e configura UDP
void wifi_init() {
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi!\n");
        return;
    }

    cyw43_arch_enable_sta_mode(); // Habilita o modo Station
    printf("Conectando ao Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar!\n");
        gpio_put(LED_PIN_GREEN, 0); // Mantém apenas o LED vermelho aceso para indicar falha
        wifi_reconnect(); // Tenta novamente
        return;
    }

    printf("Conectado!\n");
    init_irq_buttons(); // Inicializa as interrupções dos botões
    gpio_put(LED_PIN_RED, 0); // Mantém apenas o LED verde aceso para indicar sucesso

    // Configuração do UDP
    udp_conn = udp_new();
    if (udp_conn) {
        ip_addr_t ip;
        IP4_ADDR(&ip, 0, 0, 0, 0);  // Aceita pacotes de qualquer IP
        udp_bind(udp_conn, &ip, UDP_PORT); // Associa o PCB UDP à porta
        udp_recv(udp_conn, udp_receive_callback, NULL); // Configura a função de callback para recepção de pacotes
        printf("Servidor UDP configurado na porta %d\n", UDP_PORT); // Mensagem de sucesso na configuração do servidor UDP
    }
}

// Envia dados para o PC
void send_udp_packet(char *data, uint16_t len) {
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM); // Aloca um buffer para o pacote
    if (!p) return; // Verifica se houve falha na alocação
    memcpy(p->payload, data, len); // Copia os dados para o payload
    ip_addr_t dest_ip; // Endereço de destino
    ipaddr_aton(SERVER_IP, &dest_ip); // Converte o endereço IP
    udp_sendto(udp_conn, p, &dest_ip, UDP_PORT); // Envia o pacote
    pbuf_free(p); // Libera o buffer
}
