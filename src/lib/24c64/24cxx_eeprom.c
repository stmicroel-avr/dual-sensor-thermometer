#include <stdbool.h>
#include <stdint.h>

#include "../twi/twi.h"
#include "../../board/board.h"

/**
 * Прочитать один байт из внешней I2C EEPROM по абсолютному адресу
 *
 * @param addr Абсолютный адрес в EEPROM (0 .. EXT_EEPROM_SIZE-1)
 * @return Прочитанное значение байта
 */
static uint8_t eeprom_read_u8(uint16_t addr) {
    twi_start((uint8_t)(EXT_EEPROM_ADDR << 1));     // SLA+W
    twi_write((uint8_t)(addr >> 8));
    twi_write((uint8_t)(addr & 0xFF));

    twi_start((uint8_t)((EXT_EEPROM_ADDR << 1) | 1)); // SLA+R
    uint8_t v = twi_read_nack();
    twi_stop();

    return v;
}

/**
 * Прочитать 24-битное значение времени (minutes_since) из EEPROM
 *
 * @param base_addr Адрес начала записи
 * @return Количество минут с опорной даты
 */
static uint32_t eeprom_read_minutes24(uint16_t base_addr) {
    uint8_t b0 = eeprom_read_u8((uint16_t)(base_addr + 0));
    uint8_t b1 = eeprom_read_u8((uint16_t)(base_addr + 1));
    uint8_t b2 = eeprom_read_u8((uint16_t)(base_addr + 2));

    return (uint32_t)b0 | ((uint32_t)b1 << 8) | ((uint32_t)b2 << 16);
}

/**
 * Найти адрес для следующей записи в кольцевом буфере EEPROM
 *
 * Алгоритм:
 *  1. Сканирует EEPROM по шагу размера записи
 *  2. Если найден слот без FRAME_VALID_MARKER — возвращает его (есть "дырка")
 *  3. Если все слоты валидны — ищет запись с максимальным timestamp (minutes_since)
 *     и возвращает адрес следующего слота (с учётом wrap)
 *
 * Используется при старте устройства для восстановления состояния
 * кольцевого буфера.
 *
 * @param rec_len Размер одной записи в байтах (в проекте ожидается 8)
 * @return Абсолютный адрес, куда следует писать следующую запись
 */
static uint16_t find_next_write_addr_by_max_time(uint8_t rec_len) {
    uint16_t best_addr = 0;
    uint32_t best_minutes = 0;
    bool found_any = false;

    // Проходим ровно по целым слотам
    for (uint16_t base = 0; (uint16_t)(base + rec_len) <= EXT_EEPROM_SIZE; base = (uint16_t)(base + rec_len)) {
        uint16_t marker_addr = (uint16_t)(base + (rec_len - 1));
        uint8_t marker = eeprom_read_u8(marker_addr);

        // Если слот невалиден — это первая "дырка": туда и пишем.
        if (marker != FRAME_VALID_MARKER) {
            return base;
        }

        // Слот валиден, читаем timestamp и ищем максимум.
        uint32_t m = eeprom_read_minutes24(base);
        if (!found_any || (m > best_minutes)) {
            best_minutes = m;
            best_addr = base;
            found_any = true;
        }
    }

    // Если вообще ничего не нашли (теоретически почти невозможно при rec_len>0),
    // то начинаем с 0.
    if (!found_any) {
        return 0;
    }

    // Следующий слот после самого нового
    uint16_t next = (uint16_t)(best_addr + rec_len);
    if (next >= EXT_EEPROM_SIZE) {
        next = 0;
    }
    return next;
}


/**
 * Записать буффер байт в кольцевой буффер
 *
 * @param buff Указатель на буффер
 * @param len Длина буффера
 */
uint16_t write_data_to_ring_buffer(uint8_t *buff, uint8_t len) {
    buff[len - 1] = FRAME_VALID_MARKER;

    static bool is_initialized = false;
    static uint16_t mem_addr = 0;
    if (!is_initialized) {
        is_initialized = true;
        mem_addr = find_next_write_addr_by_max_time(len);
    }

    if ((uint16_t)(mem_addr + len) > EXT_EEPROM_SIZE) {
        mem_addr = 0;
    }

    twi_start(EXT_EEPROM_ADDR << 1);

    twi_write((uint8_t)(mem_addr >> 8));
    twi_write((uint8_t)(mem_addr & 0xFF));

    for (uint8_t i=0; i<len; i++) {
        twi_write(buff[i]);
    }

    twi_stop();

    mem_addr = (uint16_t)(mem_addr + len);
    return mem_addr;
}