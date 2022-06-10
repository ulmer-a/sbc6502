cmake_minimum_required(VERSION 3.14)

project(emulator VERSION 1.0)

add_library(emulator STATIC
    include/Emulator.h
    include/Registers.h
    api.cpp
    opcode_table.h
    flag_helper.h
    emulator_impl.h
    emulator_impl.cpp
    emulator.cpp
    addr_modes.cpp
    instructions.cpp
    wdc_instructions.cpp
)

install(TARGETS emulator DESTINATION .)