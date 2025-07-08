#include "eeprom.h"

void EEPROM_Init(struct EEPROM *mem, I2C_HandleTypeDef *hi2c,
                 uint16_t addr, uint16_t block_size, uint16_t block_count,
                 uint16_t page_size, uint8_t mem_addr_size)
{
    mem->addres = addr;
    mem->hi2c = hi2c;
    mem->block_size = block_size; // дефолт
    mem->block_count = block_count;
    mem->page_size = page_size;
    mem->page_count = block_size / page_size;
    mem->mem_addr_size = mem_addr_size;
}
HAL_StatusTypeDef EEPROM_Write(struct EEPROM *mem, uint16_t addr,
                               uint8_t *buffer, uint32_t n, uint32_t timeout)
{
    HAL_StatusTypeDef res;
    uint32_t bytes_written = 0;
    uint32_t delay = 0;
    while (bytes_written < n)
    {
        uint16_t current_addr = addr + bytes_written;
        uint16_t current_block = current_addr / mem->block_size;
        uint16_t offset_in_page = current_addr % mem->page_size;
        uint16_t bytes_in_page = mem->page_size - offset_in_page;
        uint16_t to_write = (n - bytes_written < bytes_in_page) ? (n - bytes_written) : bytes_in_page;
        res = HAL_I2C_Mem_Write(mem->hi2c,
                                ((mem->addres) | current_block) << 1,
                                current_addr, mem->mem_addr_size,
                                buffer + bytes_written,
                                to_write, timeout);
        if (res != HAL_OK)
        {
            return res;
        }
        // HAL_Delay(5); //в прерывания не будет работать из-за приоритета HAL_TICK (15 самый слабый)
        // поэтому сделем "очень умно"
        // 16 MGZ примерно 27к, на один проход примерно 3 иснтуркции
        while (delay < 35000)
            delay++;
        bytes_written += to_write;
        delay = 0;
    }
    return HAL_OK;
}

HAL_StatusTypeDef EEPROM_Read(struct EEPROM *mem, uint16_t addr,
                              uint8_t *buffer, uint32_t n, uint32_t timeout)
{
    HAL_StatusTypeDef res;
    uint32_t bytes_read = 0;
    uint32_t delay = 0;
    while (bytes_read < n)
    {
        uint16_t current_addr = addr + bytes_read;
        uint16_t current_block = current_addr / mem->block_size;
        uint16_t offset_in_block = current_addr % mem->block_size;
        uint16_t bytes_until_block_end = mem->block_size - offset_in_block;
        uint16_t to_read = (n - bytes_read < bytes_until_block_end) ? (n - bytes_read) : bytes_until_block_end;

        res = HAL_I2C_Mem_Read(mem->hi2c,
                               ((mem->addres) | current_block) << 1,
                               current_addr, mem->mem_addr_size,
                               buffer + bytes_read,
                               to_read, timeout);
        if (res != HAL_OK)
        {
            return res;
        }
        // HAL_Delay(5); //в прерывания не будет работать из-за приоритета HAL_TICK (15 самый слабый)
        // поэтому сделем "очень умно"
        // 16 MGZ примерно 27к, на один проход примерно 3 иснтуркции
        while (delay < 35000)
            delay++;
        bytes_read += to_read;
        delay = 0;
    }
    return HAL_OK;
}
