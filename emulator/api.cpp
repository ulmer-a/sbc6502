/* 65C02 Emulator
 * Copyright (C) 2022 Alexander Ulmer <alex@gurdinet.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "include/Emulator.h"
#include "include/Registers.h"

extern "C" {
    uint8_t emulator_read(void* mem, uint16_t addr);
    void emulator_write(void* mem, uint16_t addr, uint8_t value);
}

class MyEmulator : public em::Emulator {
    public:
        MyEmulator(void* memory)
            : m_memory(memory)
        { }

        ~MyEmulator() = default;

        inline void* getMemoryPointer() { return m_memory; }

    protected:
        uint8_t read(uint16_t addr, bool transient = false) override {
            return emulator_read(m_memory, addr);
        }

        void write(uint16_t addr, uint8_t value) override {
            emulator_write(m_memory, addr, value);
        }
    
    private:
        void* m_memory;
};

extern "C" {
    MyEmulator* emulator_create(void* memory);
    void* emulator_destroy(MyEmulator* ctx);
    void emulator_reset(MyEmulator* ctx);
    void emulator_step(MyEmulator* ctx);
    void emulator_send_irq(MyEmulator* ctx);
    void emulator_send_nmi(MyEmulator* ctx);
    em::Registers* emulator_get_registers(MyEmulator* ctx);
}

MyEmulator* emulator_create(void* memory_ptr) {
    return new MyEmulator(memory_ptr);
}

void* emulator_destroy(MyEmulator* ctx) {
    void* ptr = ctx->getMemoryPointer();
    delete ctx;
    return ptr;
}

em::Registers* emulator_get_registers(MyEmulator* ctx) {
    return &ctx->registers();
}

void emulator_reset(MyEmulator* ctx) {
    ctx->reset();
}

void emulator_step(MyEmulator* ctx) {
    ctx->step();
}

void emulator_send_irq(MyEmulator* ctx) {
    ctx->irq();
}

void emulator_send_nmi(MyEmulator* ctx) {
    ctx->nmi();
}