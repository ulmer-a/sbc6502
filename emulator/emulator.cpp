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
