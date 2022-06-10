#include "emulator_impl.h"
#include "flag_helper.h"

#include <cstdlib>
#include <cassert>

using namespace em;

void EmulatorImpl::stz()
{
    writeBack(0x00);
}

void EmulatorImpl::stp()
{
    // todo
    exit(0);
}

void EmulatorImpl::phx()
{
    push8(reg.x);
}

void EmulatorImpl::plx()
{
    reg.x = pull8();
    
    CALC_ZERO(reg.x);
    CALC_SIGN(reg.x);
}

void EmulatorImpl::phy()
{
    push8(reg.y);
}

void EmulatorImpl::ply()
{
    reg.y = pull8();
    
    CALC_ZERO(reg.y);
    CALC_SIGN(reg.y);
}

void EmulatorImpl::wai()
{
    // todo
    nop();
}

void EmulatorImpl::bra()
{
    reg.pc += rel_addr;
}

void EmulatorImpl::tsb()
{
    auto result = reg.a | getValue();
    CALC_ZERO(result);
    writeBack(result);
}

void EmulatorImpl::trb()
{
    auto result = ~reg.a & getValue();
    CALC_ZERO(result);
    writeBack(result);
}

#define rmb(n) {                \
    reg.a &= ~(n << 1);             \
}

#define smb(n) {                \
    reg.a |= (n << 1);              \
}

#define bbr(n) {                \
    if ((reg.a & (1 << n)) == 0)    \
        bra();                  \
}

#define bbs(n) {                \
    if ((reg.a & (1 << n)) != 0)    \
        bra();                  \
}

void EmulatorImpl::rmb0() rmb(0)
void EmulatorImpl::rmb1() rmb(1)
void EmulatorImpl::rmb2() rmb(2)
void EmulatorImpl::rmb3() rmb(3)
void EmulatorImpl::rmb4() rmb(4)
void EmulatorImpl::rmb5() rmb(5)
void EmulatorImpl::rmb6() rmb(6)
void EmulatorImpl::rmb7() rmb(7)

void EmulatorImpl::smb0() smb(0)
void EmulatorImpl::smb1() smb(1)
void EmulatorImpl::smb2() smb(2)
void EmulatorImpl::smb3() smb(3)
void EmulatorImpl::smb4() smb(4)
void EmulatorImpl::smb5() smb(5)
void EmulatorImpl::smb6() smb(6)
void EmulatorImpl::smb7() smb(7)

void EmulatorImpl::bbs0() bbs(0)
void EmulatorImpl::bbs1() bbs(1)
void EmulatorImpl::bbs2() bbs(2)
void EmulatorImpl::bbs3() bbs(3)
void EmulatorImpl::bbs4() bbs(4)
void EmulatorImpl::bbs5() bbs(5)
void EmulatorImpl::bbs6() bbs(6)
void EmulatorImpl::bbs7() bbs(7)

void EmulatorImpl::bbr0() bbr(0)
void EmulatorImpl::bbr1() bbr(1)
void EmulatorImpl::bbr2() bbr(2)
void EmulatorImpl::bbr3() bbr(3)
void EmulatorImpl::bbr4() bbr(4)
void EmulatorImpl::bbr5() bbr(5)
void EmulatorImpl::bbr6() bbr(6)
void EmulatorImpl::bbr7() bbr(7)
