#pragma once

#define set_carry() reg.status |= FLAG_CARRY
#define clear_carry() reg.status &= (~FLAG_CARRY)
#define set_zero() reg.status |= FLAG_ZERO
#define clear_zero() reg.status &= (~FLAG_ZERO)
#define set_interrupt() reg.status |= FLAG_INTERRUPT
#define clear_interrupt() reg.status &= (~FLAG_INTERRUPT)
#define set_decimal() reg.status |= FLAG_DECIMAL
#define clear_decimal() reg.status &= (~FLAG_DECIMAL)
#define set_overflow() reg.status |= FLAG_OVERFLOW
#define clear_overflow() reg.status &= (~FLAG_OVERFLOW)
#define set_sign() reg.status |= FLAG_SIGN
#define clear_sign() reg.status &= (~FLAG_SIGN)

#define CALC_ZERO(n) {\
    if ((n) & 0x00FF) clear_zero();\
        else set_zero();\
}

#define CALC_SIGN(n) {\
    if ((n) & 0x0080) set_sign();\
        else clear_sign();\
}

#define CALC_CARRY(n) {\
    if ((n) & 0xFF00) set_carry();\
        else clear_carry();\
}

#define CALC_OVERFLOW(n, m, o) { /* n = result, m = accumulator, o = memory */ \
    if (((n) ^ (uint16_t)(m)) & ((n) ^ (o)) & 0x0080) set_overflow();\
        else clear_overflow();\
}

#define save_to_a(n) {          \
    reg.a = (uint8_t)((n) & 0xff);  \
}
