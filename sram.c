//
// sram.h ... target sram read/write routines
//

#include "config.h"
#ifdef USE_SRAM
#if defined(__cplusplus)
extern "C" {
#endif

//
// external definition
//
#if !defined(USE_EXTSRAM)
extern uint8_t sram[SRAM_SIZE];
#endif !USE_EXTSRAM

#if !defined(USE_EXTSRAM)
void sram_begin (void)
{
  // do nothing
}

void sram_end (void)
{
  // do nothing
}

uint8_t sram_read(soff_t addr)
{
  return addr < SRAM_SIZE ? sram[addr] : 0xff;
}

void sram_write(soff_t addr, uint8_t c)
{
  if (addr < SRAM_SIZE)
    sram[addr] = c;
}

#else //!USE_EXTSRAM

#ifdef ATMEGA64
#include <Arduino.h>
#include "ucp_pindefs.h"
#endif
//
// external SRAM chip via GPIO access
//

//@sram_begin
//@param int rwflag ... read:READ(0),WRITE(1)
//
// default D0-7 mode is OUTPUT
void sram_begin(void)
{
  DDRC = DDRF = 0xff; // A0-15: output
  DDRA = 0xff;        // D0-7: output
  PORTE |= M(MREQ)|M(WR)|M(RD); 
  DDRE |= M(MREQ)|M(WR)|M(RD);   // MREQ/RD/WR output
}

void sram_end(void)
{
  DDRE &= ~(M(MREQ)|M(WR)|M(RD));   // MREQ/RD/WR input
  PORTE |= (M(MREQ)|M(WR)|M(RD));   // pullup
  DDRC = DDRF = 0x00; // A0-15: input
  DDRA = 0x00;        // D0-7: input
}

uint8_t sram_read(soff_t addr)
{
  uint8_t c;
  PORTC = (addr >> 8);
  PORTF = addr & 0xff;  // set address
  DDRA = 0;   // D0-7 input
#ifdef USE_ARDUINO_GPIO
  digitalWrite(MREQ_Pin, LOW);
  digitalWrite(RD_Pin, LOW);
#else
  PORTE &= ~(M(MREQ)|M(RD));
#endif
  _nop();
  _nop();
  _nop();
  c = PINA;       // read D0-7
#ifdef USE_ARDUINO_GPIO
  digitalWrite(RD_Pin, HIGH);
  digitalWrite(MREQ_Pin, HIGH);
#else
  PORTE |= M(MREQ)|M(RD);
#endif
  return c;
}

void sram_write(soff_t addr, uint8_t c)
{
  PORTC = (addr >> 8);
  PORTF = addr & 0xff;  // set address
  DDRA = 0xff;     // output
  PORTA = c;
#ifdef USE_ARDUINO_GPIO
  digitalWrite(MREQ_Pin, LOW);
  digitalWrite(WR_Pin, LOW);
#else
  PORTE &= ~M(MREQ);
  PORTE &= ~M(WR);
#endif
  _nop();
  _nop();
  _nop();
  _nop();
#ifdef USE_ARDUINO_GPIO
  digitalWrite(MREQ_Pin, HIGH);
  digitalWrite(WR_Pin, HIGH);
#else
  PORTE |= M(MREQ)|M(WR);
#endif
  DDRA = 0;   // D0-7 input
}
#endif //!USE_ARDUINO_GPIO

// trailing magic spells
#if defined(__cplusplus)  
}
#endif
#endif //USE_SRAM
