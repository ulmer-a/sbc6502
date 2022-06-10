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
#include "emulator_impl.h"

using namespace em;

Emulator::Emulator()
    : p_impl(std::make_unique<EmulatorImpl>(*this))
{ }

Emulator::~Emulator() {}

void Emulator::reset() {
    p_impl->reset();
}

void Emulator::step() {
    p_impl->step();
}

void Emulator::irq() {
    p_impl->irq();
}

void Emulator::nmi() {
  p_impl->nmi();
}

Registers &Emulator::registers() {
  return p_impl->registers();
}
