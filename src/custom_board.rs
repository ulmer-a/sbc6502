use super::emulator::MemoryBus;
use std::{io, io::Read, fs};

pub struct CustomBoard {
    devices: Vec<Box<dyn Device>>,
}

impl CustomBoard {
    pub fn new() -> Self {
        Self { devices: vec![] }
    }

    pub fn add_ram(&mut self, start_addr: u16, size: usize) {
        self.devices.push(Box::new(
            SimpleMemory::new(start_addr, size, false)
        ));
    }

    pub fn add_rom(&mut self, start_addr: u16, size: usize, file: &str) {
        let mut rom_device = SimpleMemory::new(start_addr, size, true);
        rom_device.load_from_file(file).unwrap();
        self.devices.push(Box::new(rom_device));
    }
}

impl MemoryBus for CustomBoard {
    fn read(&mut self, addr: u16) -> u8 {
        for dev in &mut self.devices {
            if addr >= dev.start_addr() &&
                (addr as usize) < (dev.start_addr() as usize) + dev.size() {
                return dev.read(addr - dev.start_addr());
            }
        }
        0x00
    }

    fn write(&mut self, addr: u16, value: u8) {
        for dev in &mut self.devices {
            if addr >= dev.start_addr() &&
                addr < dev.start_addr() + dev.size() as u16 {
                dev.write(addr - dev.start_addr(), value);
            }
        }
    }
}

pub trait Device: MemoryBus {
    fn start_addr(&self) -> u16;
    fn size(&self) -> usize;
}

struct SimpleMemory {
    start_addr: u16,
    memory: Vec<u8>,
    readonly: bool,
}

impl SimpleMemory {
    fn new(start_addr: u16, size: usize, readonly: bool) -> Self {
        Self {
            start_addr,
            memory: vec![0u8; size],
            readonly,
        }
    }

    fn load_from_file(&mut self, filename: &str) -> io::Result<()> {
        let mut file = fs::File::open(filename)?;
        file.read_exact(&mut self.memory)?;
        Ok(())
    }
}

impl MemoryBus for SimpleMemory {
    fn read(&mut self, addr: u16) -> u8 {
        self.memory[addr as usize]
    }

    fn write(&mut self, addr: u16, value: u8) {
        if !self.readonly {
            self.memory[addr as usize] = value;
        }
    }
}

impl Device for SimpleMemory {
    fn start_addr(&self) -> u16 {
        self.start_addr
    }

    fn size(&self) -> usize {
        self.memory.len()
    }
}