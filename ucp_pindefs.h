//
// ucp_pindefs.h
//
#ifndef __UCP_PINDEFS_H
#define __UCP_PINDEFS_H
#if defined(__cplusplus)
extern "C" {
#endif

// pin definitions
//
// pin-manipulating macros
//
#define glue(a,b) a ## b
#define xglue(a,b,c) glue(a,b) c

#define M(pin) (1<<glue(pin,_PN_))

#define inPin(pin) (xglue(PIN,pin ## _PT_, & (1<<pin ## _PN_)))
#define setPin(pin) (xglue(PORT,pin ## _PT_, |= (1<<pin ## _PN_)))
#define resetPin(pin) (xglue(PORT,pin ## _PT_, &= ~(1<<pin ## _PN_)))
#define togglePin(pin) (xglue(PORT,pin ## _PT_, ^= (1<<pin ## _PN_)))

#define TOGGLE() do{togglePin(TEST);}while(0)

#define WR_PN_   2
#define RD_PN_   3
#define MREQ_PN_ 4
#define BOOT_PN_ 5
#define BUSRQ_PN_ 6
#define WAIT_RES_PN_ 7
#define INT_PN_  4
#define RESET_PN_ (5)
#define WAIT_PN_  (6)
#define CLK_PN_  (7)
#define IORQ_PN_  (2)
#define BUSACK_PN_  (3)
#define HALT_PN_  (4)
#define USER_PN_  (5)
#define M1_PN_    (6)
#define NMI_PN_   (7)
#define A16_PN_   (0)
#define TEST_PN_  (1)

#define WR_PT_   E
#define RD_PT_   E
#define MREQ_PT_ E
#define BOOT_PT_ E
#define BUSRQ_PT_ E
#define WAIT_RES_PT_ E
#define INT_PT_  B
#define RESET_PT_ B
#define WAIT_PT_ B
#define CLK_PT_ B
#define IORQ_PT_ D
#define BUSACK_PT_  D
#define HALT_PT_ D
#define USER_PT_ D
#define M1_PT_   D
#define NMI_PT_  D
#define A16_PT_  G
#define TEST_PT_ G

// trailing magic spells
#if defined(__cplusplus)
}
#endif
#endif //__UCP_PINDEFS_H
