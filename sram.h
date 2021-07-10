//
// sram.h ... machine dependent target sram read/write
//

#ifndef __SRAM_H
#define __SRAM_H
#if defined(__cplusplus)
extern "C" {
#endif

//
//
//
#define _nop() do{asm volatile("  nop");}while(0)

void sram_begin(void);
void sram_end(void);
extern uint8_t sram_read(soff_t addr);
extern void sram_write(soff_t addr, uint8_t c);

#if defined(__cplusplus)
}
#endif

#endif //__SRAM_H
