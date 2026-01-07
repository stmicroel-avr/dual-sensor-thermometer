#include "stdint.h"
#include <avr/io.h>
#include <util/delay.h>

/**
 * Считать напряжение питания
 *
 * @return
 */
uint16_t readVcc(void) {
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // Vbg (1.1V) against Vcc
    ADCSRA |= _BV(ADEN);
    _delay_ms(2);

    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC));

    return 1125300UL / ADC;
}