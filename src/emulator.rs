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

type PtrHandle = ::libc::c_void;

#[allow(improper_ctypes)]
extern "C" {
    fn emulator_create(memory: *mut Box<dyn MemoryBus>) -> *mut PtrHandle;
    fn emulator_get_registers(ctx: *mut PtrHandle) -> *mut RegisterState;
    fn emulator_destroy(ctx: *mut PtrHandle) -> *mut Box<dyn MemoryBus>;

    fn emulator_reset(ctx: *mut PtrHandle);
    fn emulator_step(ctx: *mut PtrHandle);

    fn emulator_send_irq(ctx: *mut PtrHandle);
    fn emulator_send_nmi(ctx: *mut PtrHandle);
}

#[repr(C)]
pub struct RegisterState {
    pub pc: u16,
    pub a: u8,
    pub x: u8,
    pub y: u8,
    pub sp: u8,
    pub status: u8,
}

pub trait MemoryBus {
    fn read(&mut self, addr: u16) -> u8;
    fn write(&mut self, addr: u16, value: u8);
}

#[no_mangle]
extern "C" fn emulator_read(memory: *mut Box<dyn MemoryBus>, addr: u16) -> u8 {
    unsafe {
        let mut mem = Box::from_raw(memory);
        let value = mem.read(addr);
        Box::leak(mem);
        value
    }
}

#[no_mangle]
extern "C" fn emulator_write(memory: *mut Box<dyn MemoryBus>, addr: u16, value: u8) {
    unsafe {
        let mut mem = Box::from_raw(memory);
        mem.write(addr, value);
        Box::leak(mem);
    }
}

pub struct Emulator {
    handle: *mut PtrHandle,
    state: *mut RegisterState,
}

impl Emulator {
    pub fn new(memory: Box<dyn MemoryBus>) -> Self {
        unsafe {
            let mem_ptr = Box::into_raw(Box::new(memory));
            let handle = emulator_create(mem_ptr);
            Self {
                handle,
                state: emulator_get_registers(handle),
            }
        }
    }

    pub fn reset(&mut self) {
        unsafe {
            emulator_reset(self.handle);
        }
    }

    pub fn get_state(&mut self) -> &mut RegisterState {
        unsafe {
            &mut *self.state
        }
    }

    pub fn step(&mut self) {
        unsafe {
            emulator_step(self.handle);
        }
    }

    pub fn send_irq(&mut self) {
        unsafe {
            emulator_send_irq(self.handle);
        }
    }

    pub fn send_nmi(&mut self) {
        unsafe {
            emulator_send_nmi(self.handle);
        }
    }
}

impl Drop for Emulator {
    fn drop(&mut self) {
        unsafe {
            Box::from_raw(emulator_destroy(self.handle));
        }
    }
}
