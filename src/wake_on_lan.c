#include "mouseFunction.h"
#include "wifi_config.h"

void send_magic_packet() {
    uint8_t magic_packet[102]; // Tamanho do pacote mágico
    memset(magic_packet, 0xFF, 6); // Prefixo de sincronização

    uint8_t target_mac[6] = TARGET_MAC; // MAC do PC alvo
    for (int i = 1; i <= 16; i++) {
        memcpy(magic_packet + (i * 6), target_mac, 6); // Repete o MAC 16 vezes
    }

    struct udp_pcb *udp = udp_new(); // Cria um novo PCB UDP
    struct pbuf *packet = pbuf_alloc(PBUF_TRANSPORT, sizeof(magic_packet), PBUF_RAM); // Aloca o pacote
    if (!udp || !packet) {
        printf("Falha ao criar pacote UDP\n"); // Verifica se houve falha
        return;
    }

    memcpy(packet->payload, magic_packet, sizeof(magic_packet)); // Copia o pacote mágico para o payload

    ip_addr_t broadcast_addr; // Endereço de broadcast
    IP4_ADDR(&broadcast_addr, 255, 255, 255, 255); // Envio via Broadcast

    udp_connect(udp, &broadcast_addr, WOL_PORT); // Conecta ao endereço de broadcast
    udp_send(udp, packet); // Envia o pacote

    pbuf_free(packet); // Libera o pacote
    udp_remove(udp); // Remove o PCB UDP
    printf("Magic Packet enviado!\n"); // Mensagem de sucesso no envio do pacote mágico
}