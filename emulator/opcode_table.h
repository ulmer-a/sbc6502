#include "emulator_impl.h"

#define F(x) &EmulatorImpl::x

#define inv imp
#define zpi ind
#define absxi ind

namespace em {

static void (EmulatorImpl::*addr_table[256])() = {
   F(imp), F(indx),  F(inv),  F(inv),   F(zp),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(acc),  F(inv), F(abso), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),   F(zp),  F(zpx),  F(zpx),   F(zp),  F(imp), F(absy),  F(acc),  F(inv), F(abso), F(absx), F(absx), F(rel),
  F(abso), F(indx),  F(inv),  F(inv),   F(zp),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(acc),  F(inv), F(abso), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),  F(zpx),  F(zpx),  F(zpx),   F(zp),  F(imp), F(absy),  F(acc),  F(inv), F(absx), F(absx), F(absx), F(rel),
   F(imp), F(indx),  F(inv),  F(inv),  F(inv),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(acc),  F(inv), F(abso), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),  F(inv),  F(zpx),  F(zpx),   F(zp),  F(imp), F(absy),  F(imp),  F(inv),  F(inv), F(absx), F(absx), F(rel),
   F(imp), F(indx),  F(inv),  F(inv),   F(zp),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(acc),  F(inv),  F(ind), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),  F(zpx),  F(zpx),  F(zpx),   F(zp),  F(imp), F(absy),  F(imp),  F(inv), F(absxi), F(absx), F(absx), F(rel),
   F(rel), F(indx),  F(inv),  F(inv),   F(zp),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(imp),  F(inv), F(abso), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),  F(zpx),  F(zpx),  F(zpy),   F(zp),  F(imp), F(absy),  F(imp),  F(inv), F(abso), F(absx), F(absx), F(rel),
   F(imm), F(indx),  F(imm),  F(inv),   F(zp),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(imp),  F(inv),  F(imp), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),  F(zpx),  F(zpx),  F(zpy),   F(zp),  F(imp), F(absy),  F(imp),  F(inv), F(absx), F(absx), F(absy), F(rel),
   F(imm), F(indx),  F(inv),  F(inv),   F(zp),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(imp),  F(imp), F(abso), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),  F(inv),  F(zpx),  F(zpx),   F(zp),  F(imp), F(absy),  F(imp),  F(imp),  F(inv), F(absx), F(absx), F(rel),
   F(imm), F(indx),  F(inv),  F(inv),   F(zp),   F(zp),   F(zp),   F(zp),  F(imp),  F(imm),  F(imp),  F(inv), F(abso), F(abso), F(abso), F(rel),
   F(rel), F(indy),  F(zpi),  F(inv),  F(inv),  F(zpx),  F(zpx),   F(zp),  F(imp), F(absy),  F(imp),  F(inv),  F(inv), F(absx), F(absx), F(rel)
};

#undef inv
#define inv nop

static void (EmulatorImpl::*optable[256])() = {
    F(brk), F(ora), F(inv), F(inv), F(tsb), F(ora), F(asl), F(rmb0), F(php), F(ora), F(asl), F(inv), F(tsb), F(ora), F(asl), F(bbr0),
    F(bpl), F(ora), F(ora), F(inv), F(trb), F(ora), F(asl), F(rmb1), F(clc), F(ora), F(inc), F(inv), F(trb), F(ora), F(asl), F(bbr1),
    F(jsr), F(and_), F(inv), F(inv), F(bit), F(and_), F(rol), F(rmb2), F(plp), F(and_), F(rol), F(inv), F(bit), F(and_), F(rol), F(bbr2),
    F(bmi), F(and_), F(and_), F(inv), F(bit), F(and_), F(rol), F(rmb3), F(sec), F(and_), F(dec), F(inv), F(bit), F(and_), F(rol), F(bbr3),
    F(rti), F(eor), F(inv), F(inv), F(inv), F(eor), F(lsr), F(rmb4), F(pha), F(eor), F(lsr), F(inv), F(jmp), F(eor), F(lsr), F(bbr4),
    F(bvc), F(eor), F(eor), F(inv), F(inv), F(eor), F(lsr), F(rmb5), F(cli), F(eor), F(phy), F(inv), F(inv), F(eor), F(lsr), F(bbr5),
    F(rts), F(adc), F(inv), F(inv), F(stz), F(adc), F(ror), F(rmb6), F(pla), F(adc), F(ror), F(inv), F(jmp), F(adc), F(ror), F(bbr6),
    F(bvs), F(adc), F(adc), F(inv), F(stz), F(adc), F(ror), F(rmb7), F(sei), F(adc), F(ply), F(inv), F(jmp), F(adc), F(ror), F(bbr7),
    F(bra), F(sta), F(inv), F(inv), F(sty), F(sta), F(stx), F(smb0), F(dey), F(bit), F(txa), F(inv), F(sty), F(sta), F(stx), F(bbs0),
    F(bcc), F(sta), F(sta), F(inv), F(sty), F(sta), F(stx), F(smb1), F(tya), F(sta), F(txs), F(inv), F(stz), F(sta), F(stz), F(bbs1),
    F(ldy), F(lda), F(ldx), F(inv), F(ldy), F(lda), F(ldx), F(smb2), F(tay), F(lda), F(tax), F(inv), F(ldy), F(lda), F(ldx), F(bbs2),
    F(bcs), F(lda), F(lda), F(inv), F(ldy), F(lda), F(ldx), F(smb3), F(clv), F(lda), F(tsx), F(inv), F(ldy), F(lda), F(ldx), F(bbs3),
    F(cpy), F(cmp), F(inv), F(inv), F(cpy), F(cmp), F(dec), F(smb4), F(iny), F(cmp), F(dex), F(wai), F(cpy), F(cmp), F(dec), F(bbs4),
    F(bne), F(cmp), F(cmp), F(inv), F(inv), F(cmp), F(dec), F(smb5), F(cld), F(cmp), F(phx), F(stp), F(inv), F(cmp), F(dec), F(bbs5),
    F(cpx), F(sbc), F(inv), F(inv), F(cpx), F(sbc), F(inc), F(smb6), F(inx), F(sbc), F(nop), F(inv), F(cpx), F(sbc), F(inc), F(bbs6),
    F(beq), F(sbc), F(sbc), F(inv), F(inv), F(sbc), F(inc), F(smb7), F(sed), F(sbc), F(plx), F(inv), F(inv), F(sbc), F(inc), F(bbs7)
};

}
