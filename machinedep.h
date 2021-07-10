/*
 * machidep.h ... machine dependent functions
 *
 *  Created on: 2021/ 06 / 18
 *      Author: tenda_000
 */

#ifndef MACHIDEP_H_
#define MACHIDEP_H_
#if defined(__cplusplus)
extern "C" {
#endif

#include <Arduino.h>
#include <inttypes.h>
#include "config.h"

//
// GPIO macros and pin definitions
//
#ifdef ATMEGA64
#define glue(a,b) a ## b
#define xglue(a,b,c) glue(a,b) c

#define M(pin) (1<<glue(pin,_PN_))

#define inPin(pin) (xglue(PIN,pin ## _PT_, & (1<<pin ## _PN_)))
#define setPin(pin) (xglue(PORT,pin ## _PT_, |= (1<<pin ## _PN_)))
#define resetPin(pin) (xglue(PORT,pin ## _PT_, &= ~(1<<pin ## _PN_)))
#define togglePin(pin) (xglue(PORT,pin ## _PT_, ^= (1<<pin ## _PN_)))

#define TOGGLE() do{togglePin(TEST);}while(0)

//
// Pin definitions
//
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

//
// Serial functions
//
int _inbyte(int32_t timeout);
void _outbyte(uint8_t c);
//void printf(const char *format, ...);

//
// SRAM access
//

#define _nop() do{asm volatile("  nop");}while(0)

void sram_begin(void);
void sram_end(void);
uint8_t sram_read(soff_t addr);
void sram_write(soff_t addr, uint8_t c);
//
// SPI functions
//
void spi_xfer (uint8_t sdata, uint8_t *rdata);
void spi_cs_reset (void);
void spi_cs_set (void);
void spi_generic_command (uint8_t *sdata, int slen, uint8_t *rdata, int rlen);
int do_spi_command (int ac, arg_t *av);

#if defined(__cplusplus)
}
#endif
#endif /* MACHIDEP_H_ */
