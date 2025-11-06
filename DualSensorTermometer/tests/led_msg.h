/*
 * led_msg.h
 *
 * Created: 04.11.2025 18:56:03
 *  Author: Stepan
 */ 


#pragma once

#include "bsp/board.h"
#include "hal/hal_gpio.h"

void blinks(uint8_t duration_sec, uint8_t blinks);
void led_power_on();