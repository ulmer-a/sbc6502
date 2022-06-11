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
 *
 * 
 * Based on the Fake6502 Emulator Core v1.1
 * Copyright (c) 2011 Mike Chambers <miker00lz@gmail.com> 
 */

#include "Emulator.h"
#include "emulator_impl.h"
#include "opcode_table.h"

#include <cassert>

using namespace em;


void EmulatorImpl::reset()
{
    reg.pc = read16(RESET_VECTOR);
    reg.a = 0;
    reg.x = 0;
    reg.y = 0;
    reg.sp = 0xfd;
    reg.status |= FLAG_CONSTANT;
}

void EmulatorImpl::step()
{
    auto opcode = read8(reg.pc++);
    assert((reg.status & FLAG_CONSTANT) != 0);

    addr_mode_acc = false;
    (this->*addr_table[opcode])();
    (this->*optable[opcode])();
}

void EmulatorImpl::nmi()
{
    push16(reg.pc);
    push8(reg.status);
    reg.status |= FLAG_INTERRUPT;
    reg.pc = read16(NMI_VECTOR);
}

void EmulatorImpl::irq()
{
    push16(reg.pc);
    push8(reg.status);
    reg.status |= FLAG_INTERRUPT;
    reg.pc = read16(IRQ_BRK_VECTOR);
}

uint8_t EmulatorImpl::read8(uint16_t addr)
{
    return memory_bus.read(addr);
}

uint16_t EmulatorImpl::read16(uint16_t addr)
{
    return TO16(
        memory_bus.read(addr),
        memory_bus.read(addr +1)
    );
}

void EmulatorImpl::write8(uint16_t addr, uint8_t value)
{
    memory_bus.write(addr, value);
}

void EmulatorImpl::push8(uint8_t value)
{
    write8(BASE_STACK + (reg.sp--), value);
}

void EmulatorImpl::push16(uint16_t value)
{
    write8(BASE_STACK + reg.sp, HIGH_BYTE(value));
    write8(BASE_STACK + ((reg.sp - 1) & 0xFF), LOW_BYTE(value));
    reg.sp -= 2;
}

uint8_t EmulatorImpl::pull8()
{
    return read8(BASE_STACK + (++reg.sp));
}

uint16_t EmulatorImpl::pull16()
{
    auto value = TO16(
        read8(BASE_STACK + ((reg.sp + 1) & 0xff)),
        read8(BASE_STACK + ((reg.sp + 2) & 0xff))
    );
    reg.sp += 2;
    return value;
}

uint16_t EmulatorImpl::getValue()
{
    if (addr_mode_acc)
        return reg.a;
    return (uint16_t)read8(eff_addr);
}

void EmulatorImpl::writeBack(uint16_t value)
{
    if (addr_mode_acc)
        reg.a = LOW_BYTE(value);
    else
        write8(eff_addr, LOW_BYTE(value));
}
