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
#include <memory>

namespace em {

    class EmulatorImpl;
    struct Registers;

    class Emulator
    {
      friend class EmulatorImpl;

      public:
        Emulator();
        virtual ~Emulator();

        void reset();
        void step();

        void irq();
        void nmi();

        Registers& registers();

      protected:
        virtual uint8_t read(uint16_t addr, bool transient = false) = 0;
        virtual void write(uint16_t addr, uint8_t value) = 0;

      private:
        std::unique_ptr<EmulatorImpl> p_impl;
    };

}
