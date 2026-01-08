#include <avr/io.h>

/**
 * Инициализация аппаратного I2C
 */
void twi_init(void) {
    TWSR = 0;
    TWBR = 32;
    TWCR = 1<<TWEN;
}

/**
 * Начать передачу данных
 *
 * @param addr Адрес slave устройства
 */
void twi_start(uint8_t addr) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    TWDR = addr;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

/**
 * Остановить прием/передачу через I2C
 */
void twi_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    while (TWCR & (1<<TWSTO));
}

/**
 * Отправить набор байт
 *
 * @param data Данные
 */
void twi_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

/**
 * Прочитать байт данных
 *
 * @param ack Бит подтвержения окончания передачи
 * @return
 */
static uint8_t twi_read(uint8_t ack) {
    if (ack) {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    } else {
        TWCR = (1 << TWINT) | (1 << TWEN);
    }

    while (!(TWCR & (1 << TWINT)));

    return TWDR;
}

/**
 * Прочитать байт с ACK (ожидаем ещё данные от slave)
 */
uint8_t twi_read_ack(void) {
    return twi_read(1);
}

/**
 * Прочитать байт с NACK (это последний байт)
 */
uint8_t twi_read_nack(void) {
    return twi_read(0);
}