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

#include "emulator_impl.h"
#include "flag_helper.h"

using namespace em;

void EmulatorImpl::adc()
{
    auto value = getValue();
    auto result = (uint16_t)reg.a + value + (uint16_t)(reg.status & FLAG_CARRY);
   
    CALC_CARRY(result);
    CALC_ZERO(result);
    CALC_OVERFLOW(result, reg.a, value);
    CALC_SIGN(result);
   
    save_to_a(result);
}

void EmulatorImpl::and_()
{
    auto result = (uint16_t)reg.a & getValue();
   
    CALC_ZERO(result);
    CALC_SIGN(result);
   
    save_to_a(result);
}

void EmulatorImpl::asl()
{
    auto result = getValue() << 1;

    CALC_CARRY(result);
    CALC_ZERO(result);
    CALC_SIGN(result);
   
    writeBack(result);
}

void EmulatorImpl::bcc()
{
    if ((reg.status & FLAG_CARRY) == 0) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::bcs()
{
    if ((reg.status & FLAG_CARRY) == FLAG_CARRY) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::beq()
{
    if ((reg.status & FLAG_ZERO) == FLAG_ZERO) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::bit()
{
    auto value = getValue();
    auto result = (uint16_t)reg.a & value;
   
    CALC_ZERO(result);

    reg.status = (reg.status & 0x3F) | (uint8_t)(value & 0xC0);
}

void EmulatorImpl::bmi()
{
    if ((reg.status & FLAG_SIGN) == FLAG_SIGN) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::bne()
{
    if ((reg.status & FLAG_ZERO) == 0) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::bpl()
{
    if ((reg.status & FLAG_SIGN) == 0) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::brk()
{
    reg.pc++;
    push16(reg.pc); // push next instruction address onto stack
    push8(reg.status | FLAG_BREAK); // push CPU reg.status to stack
    set_interrupt(); //set interrupt flag
    reg.pc = read16(IRQ_BRK_VECTOR);
}

void EmulatorImpl::bvc()
{
    if ((reg.status & FLAG_OVERFLOW) == 0) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::bvs()
{
    if ((reg.status & FLAG_OVERFLOW) == FLAG_OVERFLOW) {
        reg.pc += rel_addr;
    }
}

void EmulatorImpl::clc()
{
    clear_carry();
}

void EmulatorImpl::cld()
{
    clear_decimal();
}

void EmulatorImpl::cli()
{
    clear_interrupt();
}

void EmulatorImpl::clv()
{
    clear_overflow();
}

void EmulatorImpl::cmp()
{
    auto value = getValue();
    auto result = (uint16_t)reg.a - value;
   
    if (reg.a >= (uint8_t)(value & 0x00FF))
        set_carry();
    else
        clear_carry();

    if (reg.a == (uint8_t)(value & 0x00FF))
        set_zero();
    else
        clear_zero();

    CALC_SIGN(result);
}

void EmulatorImpl::cpx()
{
    auto value = getValue();
    auto result = (uint16_t)reg.x - value;
   
    if (reg.x >= (uint8_t)(value & 0x00FF))
        set_carry();
    else
        clear_carry();

    if (reg.x == (uint8_t)(value & 0x00FF))
        set_zero();
    else
        clear_zero();

    CALC_SIGN(result);
}

void EmulatorImpl::cpy()
{
    auto value = getValue();
    auto result = (uint16_t)reg.y - value;
   
    if (reg.y >= (uint8_t)(value & 0x00FF))
        set_carry();
    else
        clear_carry();

    if (reg.y == (uint8_t)(value & 0x00FF))
        set_zero();
    else
        clear_zero();

    CALC_SIGN(result);
}

void EmulatorImpl::dec()
{
    auto result = getValue() - 1;
   
    CALC_ZERO(result);
    CALC_SIGN(result);
   
    writeBack(result);
}

void EmulatorImpl::dex()
{
    reg.x--;
   
    CALC_ZERO(reg.x);
    CALC_SIGN(reg.x);
}

void EmulatorImpl::dey()
{
    reg.y--;
   
    CALC_ZERO(reg.y);
    CALC_SIGN(reg.y);
}

void EmulatorImpl::eor()
{
    auto result = (uint16_t)reg.a ^ getValue();
   
    CALC_ZERO(result);
    CALC_SIGN(result);
   
    save_to_a(result);
}

void EmulatorImpl::inc()
{
    auto result = getValue() + 1;
   
    CALC_ZERO(result);
    CALC_SIGN(result);
   
    writeBack(result);
}

void EmulatorImpl::inx()
{
    reg.x++;
   
    CALC_ZERO(reg.x);
    CALC_SIGN(reg.x);
}

void EmulatorImpl::iny()
{
    reg.y++;
   
    CALC_ZERO(reg.y);
    CALC_SIGN(reg.y);
}

void EmulatorImpl::jmp()
{
    reg.pc = eff_addr;
}

void EmulatorImpl::jsr()
{
    push16(reg.pc - 1);
    reg.pc = eff_addr;
}

void EmulatorImpl::lda()
{
    reg.a = (uint8_t)(getValue() & 0x00FF);
   
    CALC_ZERO(reg.a);
    CALC_SIGN(reg.a);
}

void EmulatorImpl::ldx()
{
    reg.x = (uint8_t)(getValue() & 0x00FF);
   
    CALC_ZERO(reg.x);
    CALC_SIGN(reg.x);
}

void EmulatorImpl::ldy()
{
    auto value = getValue();
    reg.y = (uint8_t)(value & 0x00FF);
   
    CALC_ZERO(reg.y);
    CALC_SIGN(reg.y);
}

void EmulatorImpl::lsr()
{
    auto value = getValue();
    auto result = value >> 1;
   
    if (value & 1)
        set_carry();
    else
        clear_carry();

    CALC_ZERO(result);
    CALC_SIGN(result);
   
    writeBack(result);
}

void EmulatorImpl::nop()
{
    // nix
}

void EmulatorImpl::ora()
{
    auto value = getValue();
    auto result = (uint16_t)reg.a | value;
   
    CALC_ZERO(result);
    CALC_SIGN(result);
   
    save_to_a(result);
}

void EmulatorImpl::pha()
{
    push8(reg.a);
}

void EmulatorImpl::php()
{
    push8(reg.status | FLAG_BREAK);
}

void EmulatorImpl::pla()
{
    reg.a = pull8();
   
    CALC_ZERO(reg.a);
    CALC_SIGN(reg.a);
}

void EmulatorImpl::plp()
{
    reg.status = pull8() | FLAG_CONSTANT;
}

void EmulatorImpl::rol()
{
    auto value = getValue();
    auto result = (value << 1) | (reg.status & FLAG_CARRY);
   
    CALC_CARRY(result);
    CALC_ZERO(result);
    CALC_SIGN(result);
   
    writeBack(result);
}

void EmulatorImpl::ror()
{
    auto value = getValue();
    auto result = (value >> 1) | ((reg.status & FLAG_CARRY) << 7);
   
    if (value & 1)
        set_carry();
    else
        clear_carry();

    CALC_ZERO(result);
    CALC_SIGN(result);
   
    writeBack(result);
}

void EmulatorImpl::rti()
{
    reg.status = pull8();
    reg.pc = pull16();
}

void EmulatorImpl::rts()
{
    reg.pc = pull16() + 1;
}

void EmulatorImpl::sbc()
{
    auto value = getValue() ^ 0x00FF;
    auto result = (uint16_t)reg.a + value + (uint16_t)(reg.status & FLAG_CARRY);
   
    CALC_CARRY(result);
    CALC_ZERO(result);
    CALC_OVERFLOW(result, reg.a, value);
    CALC_SIGN(result);
   
    save_to_a(result);
}

void EmulatorImpl::sec()
{
    set_carry();
}

void EmulatorImpl::sed()
{
    set_decimal();
}

void EmulatorImpl::sei()
{
    set_interrupt();
}

void EmulatorImpl::sta()
{
    writeBack(reg.a);
}

void EmulatorImpl::stx()
{
    writeBack(reg.x);
}

void EmulatorImpl::sty()
{
    writeBack(reg.y);
}

void EmulatorImpl::tax()
{
    reg.x = reg.a;
   
    CALC_ZERO(reg.x);
    CALC_SIGN(reg.x);
}

void EmulatorImpl::tay()
{
    reg.y = reg.a;
   
    CALC_ZERO(reg.y);
    CALC_SIGN(reg.y);
}

void EmulatorImpl::tsx()
{
    reg.x = reg.sp;
   
    CALC_ZERO(reg.x);
    CALC_SIGN(reg.x);
}

void EmulatorImpl::txa()
{
    reg.a = reg.x;
   
    CALC_ZERO(reg.a);
    CALC_SIGN(reg.a);
}

void EmulatorImpl::txs()
{
    reg.sp = reg.x;
}

void EmulatorImpl::tya()
{
    reg.a = reg.y;
   
    CALC_ZERO(reg.a);
    CALC_SIGN(reg.a);
}
