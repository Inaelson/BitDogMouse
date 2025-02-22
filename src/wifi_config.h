#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#define WIFI_SSID "YOUR SSID HERE" // Substitua pelo nome da rede Wi-Fi
#define WIFI_PASS "12345678" // Substitua pela senha da rede Wi-Fi
#define SERVER_IP "YOUR IP HERE" // Substitua pelo IP do PC alvo
#define UDP_PORT 5005 // Porta padrão do servidor UDP
#define WOL_PORT 9  // Porta padrão do Wake-on-LAN
#define TARGET_MAC {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // Substitua pelo MAC do PC alvo

#define MODE_MOUSE 0 // Modo mouse
#define MODE_WOL 1 // Modo Wake-on-LAN

#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"
#include "pico/cyw43_arch.h"

extern volatile int current_mode; // Modo atual

// Funções disponíveis no módulo
void wifi_init();
void send_udp_packet(char *data, uint16_t len);
void send_magic_packet();


#endif
