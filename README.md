# 65C02 Emulator

This package contains Rust safe bindings for my 65C02 Emulator written in C++ along
with a simple example to illustrate usage.

The emulator is originally based on the `fake6502` emulator project by Mike Chambers. I
have converted his hard work into C++ classes to get rid of global data and updated the
opcode and addressing mode tables to support the WDC 65c02 processor's instruction set.
