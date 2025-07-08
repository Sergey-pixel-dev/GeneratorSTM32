#ifndef EEPROM_LIB
#define EEPROM_LIB
#include "stdint.h"
#include "stm32f1xx_hal.h"
//   главная проблема - отсутствие DMA, надо бы потом добавить (это смерть)
//   проблема(или нет) - запись и чтение последовательности (цепчоки)
//   доступна лишь по странично (например, с середины страницы чтение или запись
//   начать нельзя). мб в будущем это, если нужно, исправить
struct EEPROM
{
    I2C_HandleTypeDef *hi2c;
    uint16_t addres;
    // uint8_t timeOut;
    uint16_t block_size; // в байтах
    uint16_t page_size;  // в байтах
    uint16_t block_count;
    uint16_t page_count; // кол-во страниц в одном блоке
    uint16_t mem_addr_size;
    // uint32_t mem_size;   // в килоБИТАХ
};

extern void EEPROM_Init(struct EEPROM *mem, I2C_HandleTypeDef *i2c,
                        uint16_t addr, uint16_t block_size, uint16_t block_count,
                        uint16_t page_size, uint8_t mem_addr_size);

extern HAL_StatusTypeDef EEPROM_Write(struct EEPROM *mem, uint16_t page_index,
                                      uint8_t *buffer, uint32_t n, uint32_t timeout);
extern HAL_StatusTypeDef EEPROM_Read(struct EEPROM *mem, uint16_t addr,
                                     uint8_t *buffer, uint32_t n, uint32_t timeout);

#endif