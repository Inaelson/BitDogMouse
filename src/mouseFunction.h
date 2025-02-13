#ifndef MOUSEFUNCTION_H
#define MOUSEFUNCTION_H

#include <stdint.h>

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define JOYSTICK_Y_PIN 26
#define JOYSTICK_X_PIN 27
#define JOYSTICK_BUTTON_PIN 22

void setup_mouse_function();
void mouse_irq_handler(uint8_t gpio, uint32_t events);

#endif