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

#pragma once

#include <stdint.h>
#include "include/Registers.h"

#define BASE_STACK     0x100

#define FLAG_CARRY     0x01
#define FLAG_ZERO      0x02
#define FLAG_INTERRUPT 0x04
#define FLAG_DECIMAL   0x08
#define FLAG_BREAK     0x10
#define FLAG_CONSTANT  0x20
#define FLAG_OVERFLOW  0x40
#define FLAG_SIGN      0x80

// interrupt and reset vector locations
#define NMI_VECTOR      0xfffa
#define RESET_VECTOR    0xfffc
#define IRQ_BRK_VECTOR  0xfffe

// word size conversion helper macros
#define LOW_BYTE(x)     ((uint8_t)((x) & 0xff))
#define HIGH_BYTE(x)    ((uint8_t)((x) >> 8))
#define TO16(lo, hi)    ((uint16_t)(lo) | ((uint16_t)(hi) << 8))


namespace em {

  class Emulator;

  class EmulatorImpl
  {
    public:
      EmulatorImpl(Emulator& parent)
        : memory_bus(parent)
      { }

      ~EmulatorImpl() = default;

      void reset();
      void step();

      void irq();
      void nmi();

      Registers reg;
      Registers& registers() { return reg; }

      // Emulator implements read(), write()
      Emulator& memory_bus;

      // memory operations
      uint8_t read8(uint16_t addr);
      uint16_t read16(uint16_t addr);
      void write8(uint16_t addr, uint8_t value);
      void write16(uint16_t addr, uint16_t value);

      // stack operations
      uint8_t pull8();
      uint16_t pull16();
      void push8(uint8_t value);
      void push16(uint16_t value);

      // addressing mode operations
      bool addr_mode_acc;
      uint16_t eff_addr, rel_addr;
      uint16_t getValue();
      void writeBack(uint16_t value);

      void imp(); void acc();
      void imm(); void rel();
      void zp(); void zpx(); void zpy();
      void abso(); void absx(); void absy();
      void ind(); void indx(); void indy();

      // base 6502 instructions
      void adc(); void and_(); void asl(); void bcc(); void bcs(); void beq();
      void bit(); void bmi();  void bne(); void bpl(); void brk(); void bvc();
      void bvs(); void clc();  void cld(); void cli(); void clv(); void cmp();
      void cpx(); void cpy();  void dec(); void dex(); void dey(); void eor();
      void inc(); void inx();  void iny(); void jmp(); void jsr(); void lda();
      void ldx(); void ldy();  void lsr(); void nop(); void ora(); void pha();
      void php(); void pla();  void plp(); void rol(); void ror(); void rti();
      void rts(); void sbc();  void sec(); void sed(); void sei(); void sta();
      void stx(); void sty();  void tax(); void tay(); void tsx(); void txa();
      void txs(); void tya();

      // 65c02 instructions
      void stz(); void stp(); void phx(); void plx(); void phy(); void ply();
      void wai(); void bra(); void tsb(); void trb();
      void bbr0(); void bbr1(); void bbr2(); void bbr3();
      void bbr4(); void bbr5(); void bbr6(); void bbr7();
      void bbs0(); void bbs1(); void bbs2(); void bbs3();
      void bbs4(); void bbs5(); void bbs6(); void bbs7();
      void smb0(); void smb1(); void smb2(); void smb3();
      void smb4(); void smb5(); void smb6(); void smb7();
      void rmb0(); void rmb1(); void rmb2(); void rmb3();
      void rmb4(); void rmb5(); void rmb6(); void rmb7();

  };
}
