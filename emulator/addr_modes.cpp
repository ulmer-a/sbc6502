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

#include "emulator_impl.h"

using namespace em;

void EmulatorImpl::imp() { }

void EmulatorImpl::acc() {
    addr_mode_acc = true;
}

void EmulatorImpl::imm() {
    // imm
    eff_addr = reg.pc++;
}

void EmulatorImpl::zp() {
    // zp
    eff_addr = (uint16_t)read8(reg.pc++);
}

void EmulatorImpl::zpx() {
    // zp, reg.x
    // zero-page wraparound
    eff_addr = LOW_BYTE((uint16_t)read8(reg.pc++) + (uint16_t)reg.x);
}

void EmulatorImpl::zpy() {
    // zp, reg.y
    // zero-page wraparound
    eff_addr = LOW_BYTE((uint16_t)read8(reg.pc++) + (uint16_t)reg.y);
}

void EmulatorImpl::rel() {
    // relative for branch ops
    // 8-bit immediate value, sign-extended
    rel_addr = (uint16_t)read8(reg.pc++);
    if (rel_addr & 0x80) rel_addr |= 0xff00;
}

void EmulatorImpl::abso() {
    // absolute
    eff_addr = read16(reg.pc);
    reg.pc += 2;
}

void EmulatorImpl::absx() {
    // absolute, reg.x
    eff_addr = read16(reg.pc) + (uint16_t)reg.x;
    reg.pc += 2;
}

void EmulatorImpl::absy() {
    // absolute, reg.y
    eff_addr = read16(reg.pc) + (uint16_t)reg.y;
    reg.pc += 2;
}

void EmulatorImpl::ind() {
    // (indirect)
    // replicate 6502 page-boundary wraparound bug
    auto addr_base = read16(reg.pc);
    auto addr_wrap = (addr_base & 0xFF00) | ((addr_base + 1) & 0x00FF);
    eff_addr = TO16(read8(addr_base), read8(addr_wrap));
    reg.pc += 2;
}

void EmulatorImpl::indx() {
    // (zp indirect, reg.x)
    // zero-page wraparound for table pointer
    auto addr = (uint16_t)(((uint16_t)read8(reg.pc++) + (uint16_t)reg.x) & 0xFF);
    eff_addr = TO16(read8(addr & 0x00FF), read8((addr + 1) & 0x00FF));
}

void EmulatorImpl::indy() {
    // (zp indirect), reg.y
    // zero-page wraparound
    auto eahelp = (uint16_t)read8(reg.pc++);
    auto eahelp2 = (eahelp & 0xFF00) | ((eahelp + 1) & 0x00FF); 
    eff_addr = (uint16_t)read8(eahelp) | ((uint16_t)read8(eahelp2) << 8);
    eff_addr += (uint16_t)reg.y;
}

