#pragma once

#include <avr/io.h>

// CPU CONF
 #define F_CPU 8000000UL

// LED CONF
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_PIN PB1

// DS18B20
#define DS_A  PD0
#define DS_B  PD1
#define DS_RESOLUTION 12

// BUTTON CONF
#define BTN_PORT PORTD
#define BTN_DDR  DDRD
#define BTN_PIN  PD2


// APP CONF
#define DISPLAY_ACTIVE_MS_TIME 10000