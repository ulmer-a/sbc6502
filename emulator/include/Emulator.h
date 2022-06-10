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
