/*
 * atmega328pb.h
 *
 * Created: 03.11.2025 20:16:24
 *  Author: stmicroel-avr
 */ 


#pragma once
#include <avr/io.h>

#define F_CPU 1000000UL
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_PIN PB1