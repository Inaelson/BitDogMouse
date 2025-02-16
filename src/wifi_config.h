#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#define WIFI_SSID "Inaelson-2.4G"
#define WIFI_PASS "21102000"
#define SERVER_IP "192.168.18.2"
#define UDP_PORT 5005

#include "lwip/udp.h"

// Variável global para a conexão UDP
extern struct udp_pcb *udp_conn;

// Funções disponíveis no módulo
void wifi_init();
void send_udp_packet(char *data, uint16_t len);


#endif
