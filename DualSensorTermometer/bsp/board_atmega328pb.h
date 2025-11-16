#pragma once

#include <avr/io.h>

// CPU CONF
#define F_CPU 8000000UL

// LED CONF
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_PIN PB1

// DS18B20
#define DS_A  PD2
#define DS_B  PD3