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

#pragma once

#define set_carry() reg.status |= FLAG_CARRY
#define clear_carry() reg.status &= (~FLAG_CARRY)
#define set_zero() reg.status |= FLAG_ZERO
#define clear_zero() reg.status &= (~FLAG_ZERO)
#define set_interrupt() reg.status |= FLAG_INTERRUPT
#define clear_interrupt() reg.status &= (~FLAG_INTERRUPT)
#define set_decimal() reg.status |= FLAG_DECIMAL
#define clear_decimal() reg.status &= (~FLAG_DECIMAL)
#define set_overflow() reg.status |= FLAG_OVERFLOW
#define clear_overflow() reg.status &= (~FLAG_OVERFLOW)
#define set_sign() reg.status |= FLAG_SIGN
#define clear_sign() reg.status &= (~FLAG_SIGN)

#define CALC_ZERO(n) {\
    if ((n) & 0x00FF) clear_zero();\
        else set_zero();\
}

#define CALC_SIGN(n) {\
    if ((n) & 0x0080) set_sign();\
        else clear_sign();\
}

#define CALC_CARRY(n) {\
    if ((n) & 0xFF00) set_carry();\
        else clear_carry();\
}

#define CALC_OVERFLOW(n, m, o) { /* n = result, m = accumulator, o = memory */ \
    if (((n) ^ (uint16_t)(m)) & ((n) ^ (o)) & 0x0080) set_overflow();\
        else clear_overflow();\
}

#define save_to_a(n) {          \
    reg.a = (uint8_t)((n) & 0xff);  \
}
