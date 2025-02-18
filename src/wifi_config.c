#include "wifi_config.h"
#include "mouseFunction.h"

struct udp_pcb *udp_conn; // Conexão UDP
struct repeating_timer timer_read_adc; // Timer para leitura do ADC

bool wifi_connected = false;

// Função de callback para recepção de pacotes
static void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    if (p != NULL) {
        printf("Pacote recebido! Tamanho: %d\n", p->tot_len);
        pbuf_free(p);
    }
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
        return;
    }

    printf("Conectado!\n");
    wifi_connected = true;
    // Adiciona um timer para leitura do ADC
    add_repeating_timer_us(-SAMPLE_RATE, read_joystick_adc, NULL, &timer_read_adc);
    gpio_put(LED_PIN_RED, 0); // Mantém apenas o LED verde aceso para indicar sucesso

    // Configuração do UDP
    udp_conn = udp_new();
    if (udp_conn) {
        ip_addr_t ip;
        IP4_ADDR(&ip, 0, 0, 0, 0);  // Aceita pacotes de qualquer IP
        udp_bind(udp_conn, &ip, UDP_PORT);
        udp_recv(udp_conn, udp_receive_callback, NULL);
        printf("Servidor UDP configurado na porta %d\n", UDP_PORT);
    }
}

// Envia dados para o PC
void send_udp_packet(char *data, uint16_t len) {
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    if (!p) return;
    memcpy(p->payload, data, len);
    ip_addr_t dest_ip;
    ipaddr_aton(SERVER_IP, &dest_ip);
    udp_sendto(udp_conn, p, &dest_ip, UDP_PORT);
    pbuf_free(p);
}
