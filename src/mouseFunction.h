#ifndef MOUSEFUNCTION_H
#define MOUSEFUNCTION_H

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/adc.h"
#include "hardware/timer.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define JOYSTICK_Y_PIN 27
#define JOYSTICK_X_PIN 26
#define JOYSTICK_BUTTON_PIN 22

#define LED_PIN_RED 13
#define LED_PIN_GREEN 11

extern uint16_t adc_buffer[2];

extern uint sm;
extern PIO pio;

void setup_buttons_function();
void setup_joystick_function();
void mouse_irq_handler(uint gpio, uint32_t events);

#endif