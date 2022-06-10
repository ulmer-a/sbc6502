mod emulator;

use std::{io::Read, fs::File};
pub use emulator::{Emulator, RegisterState, MemoryBus};

const RAM_SIZE: usize = 32*1024;
const ROM_SIZE: usize = 8*1024;

struct MyBoardMemory {
    ram: [u8; 32*1024],
    rom: [u8; 8*1024],
}

impl MyBoardMemory {
    fn new() -> Self {
        Self {
            ram: [0u8; RAM_SIZE],
            rom: [0u8; ROM_SIZE],
        }
    }

    fn load_rom(&mut self, rom_file: &str) {
        let mut file = File::open(rom_file).expect("cannot open file");
        file.read_exact(&mut self.rom).expect("can't read rom from file");       
    }
}

impl MemoryBus for MyBoardMemory {
    fn read(&mut self, addr: u16) -> u8 {
        if addr < 0x8000 {
            self.ram[addr as usize]
        } else if addr >= 0xe000 {
            self.rom[(addr - 0xe000) as usize]
        } else {
            0x00
        }
    }

    fn write(&mut self, addr: u16, value: u8) {
        if addr < 0x8000 {
            self.ram[addr as usize] = value;
        } else if addr >= 0xe000 {
            self.rom[(addr - 0xe000) as usize] = value;
        }
    }
}

fn main() {
    // create a memory map with ROM and RAM and load some file into the ROM
    let mut mem = MyBoardMemory::new();
    let filename = std::env::args().skip(1).next().unwrap();
    mem.load_rom(&filename);

    // create the CPU and give it a handle to our memory
    let mut emu = Emulator::new(Box::new(mem));

    emu.reset();
    loop {
        emu.step();
        println!("PC: 0x{:04x}", emu.get_state().pc)
    }
}
