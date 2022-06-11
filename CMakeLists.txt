cmake_minimum_required(VERSION 3.14)

project(emulator VERSION 1.1)

add_library(emulator STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Emulator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Registers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/opcode_table.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/flag_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/emulator_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/emulator_impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/emulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/addr_modes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/instructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/wdc_instructions.cpp
)

target_include_directories(emulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set(EMULATOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

# required for build with cargo
# install(TARGETS emulator DESTINATION .)

