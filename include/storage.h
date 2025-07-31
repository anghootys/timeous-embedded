#ifndef __STORAGE_H__
#define __STORAGE_H__
#include "Arduino.h"

// 8 bytes for hour and minute: HHHHMMMM
#define EEPROM_ALLOCATED_DEFAULT_SIZE (2 * 4)

typedef uint32_t mem_addr_t;

class Storage {
    size_t eeprom_allocated_size = EEPROM_ALLOCATED_DEFAULT_SIZE;
    mem_addr_t cursor = 0x0;

public:
    Storage() = default;
    Storage(size_t allocation_size) : eeprom_allocated_size(allocation_size) {};

    void initialize();

    // continuously writing to memory
    void write(char *buf, size_t len);

    void reset_cursor();
    void move_cursor(mem_addr_t addr);
    void clear_area();

    char *read_all();
};

#endif // __STORAGE_H__
