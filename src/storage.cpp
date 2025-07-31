#include "storage.h"
#include <EEPROM.h>
#include "core.h"
#include "logger.h"

#define LOG_STORAGE_CTX "STORAGE"

LoggerFacade storage_logger(Logger::setup(LOG_STORAGE_CTX));

void
Storage::initialize() {
    EEPROM.begin(this->eeprom_allocated_size);

    if (EEPROM.length() < this->eeprom_allocated_size) {
        storage_logger.error("allocated size is less than actually allocated size by eeprom lib.");
        storage_logger.error("allocated size:");
        storage_logger.error(String(this->eeprom_allocated_size).c_str());
        storage_logger.error("eeprom len:");
        storage_logger.error(String(EEPROM.length()).c_str());

        reset_processor();
    }
}

void
Storage::write(char *buf, size_t len) {
    storage_logger.info("writing into flash memory.");

    if (len > this->eeprom_allocated_size - this->cursor) {
        storage_logger.error("cannot write data to flash memory. data size on flash:");
        storage_logger.error((String(this->eeprom_allocated_size - this->cursor) + String(" bytes.")).c_str());
        storage_logger.error("data you want to write:");
        storage_logger.error((String(len) + String(" bytes.")).c_str());
    } else {
        size_t i = 0;
        while (--len)
            EEPROM.write(this->cursor++, buf[i++]);
        EEPROM.commit();

        storage_logger.success("data written into flash memory successfully.");
    }
}

void
Storage::reset_cursor() {
    this->cursor = 0x0;
}

void
Storage::move_cursor(mem_addr_t addr) {
    storage_logger.info("moving cursor from address:");
    storage_logger.info(String(this->cursor).c_str());
    storage_logger.info("to address:");
    storage_logger.info(String(addr).c_str());

    if (addr < this->eeprom_allocated_size)
        this->cursor = addr;

    storage_logger.success("cursor moved successfully.");
}

void
Storage::clear_area() {
    storage_logger.info("clearing flash memory.");

    this->reset_cursor();

    for (; this->cursor < this->eeprom_allocated_size; this->cursor++)
        EEPROM.write(this->cursor, 0x0);

    this->reset_cursor();

    storage_logger.success("flash memory cleand successfully.");
}

char *
Storage::read_all() {
    storage_logger.info("reading flash memory.");

    char *buf = alloc<char>(this->eeprom_allocated_size);

    mem_addr_t c = 0;
    for (; c < this->eeprom_allocated_size; c++)
        EEPROM.get<char>((int) c, buf[c]);

    storage_logger.success("memory read successfully.");

    return buf;
}
