#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#define WIFI_SSID "Inaelson-2.4G"
#define WIFI_PASS "21102000"
#define SERVER_IP "192.168.18.2"
#define UDP_PORT 5005
#define WOL_PORT 9  // Porta padrão do Wake-on-LAN
#define TARGET_MAC {0x00, 0xE0, 0x70, 0xCF, 0x5B, 0x0B} // Substitua pelo MAC do PC alvo

#define MODE_MOUSE 0 // Modo mouse
#define MODE_WOL 1 // Modo Wake-on-LAN

#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"
#include "pico/cyw43_arch.h"

// Variável global para a conexão UDP
extern struct udp_pcb *udp_conn;

extern volatile bool server_active; // Flag que indica se o servidor está ativo
extern volatile int current_mode; // Modo atual

// Funções disponíveis no módulo
void wifi_init();
void send_udp_packet(char *data, uint16_t len);
void send_magic_packet();


#endif
