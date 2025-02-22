#ifndef MOUSEFUNCTION_H
#define MOUSEFUNCTION_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
#include "hardware/timer.h"

#define SAMPLE_RATE 70000 // Taxa de amostragem do ADC em microssegundos (70ms)

#define BUTTON_A_PIN 5 // Pino do botão A
#define BUTTON_B_PIN 6 // Pino do botão B
#define JOYSTICK_Y_PIN 26 // Pino do eixo Y do Joystick
#define JOYSTICK_X_PIN 27 // Pino do eixo X do Joystick
#define JOYSTICK_BUTTON_PIN 22 // Pino do botão do Joystick

#define LED_PIN_RED 13 // Pino do LED vermelho
#define LED_PIN_GREEN 11 // Pino do LED verde
#define LED_PIN_BLUE 12 // Pino do LED azul

void setup_buttons_function(); // Função para configurar os botões
void setup_joystick_function(); // Função para configurar o joystick
void init_irq_buttons(); // Função para inicializar as interrupções dos botões
void mouse_irq_handler(uint gpio, uint32_t events); // Manipulador de interrupção do mouse
bool read_joystick_adc(struct repeating_timer *t); // Função para leitura do ADC

#endif