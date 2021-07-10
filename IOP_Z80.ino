
//
// CPUTest for UCP boards
//

#include "config.h"
#include "ucp_pindefs.h"
#include <stdlib.h>

//
// CLK ... Counter/Timer2
// OC2/PB7: output pin
//
void initCLK(void)
{
  TCCR2 = 0x18;
        // CS22:0 = 000 stop counter
        // WGM21:0 = 10
        // COM21:0 = 01
}

void startCLK(void)
{
  TCNT2 = OCR2 = 3;
  // 3 ... 2.5MHz
  // 4 ... 2MHz
  // 9 ... 1MHz
  // 19 .. 500kHz
  // 49 .. 200kHz
  // 99 .. 100kHz
  TCCR2 |= 0x01;    // set COM21:0 as CTC mode
      // 00 .. stop counter
      // 01 .. 1/1
      // 02 .. 1/8
      // 03 .. 1/64
      // 04 .. 1/256
      // 05 .. 1/1024
}

void oneCLK(int num)
{
  int nopcnt = 5;
  for (int i = 0; i < num; ++i) {
    setPin(CLK);
    for (int j = 0; j < nopcnt; ++j) _nop();
    resetPin(CLK);
    for (int j = 0; j < nopcnt; ++j) _nop();
  }
}

//
//
//
//
// IOP .. do in/out instructions
//

void iop (void)
{
  while (1) {
    if (PINB & M(WAIT)) {
      continue;
    }
    // WAIT active
    TOGGLE();
    uint8_t addr = PINF;
    if ((PINE & M(RD)) == 0) {
      TOGGLE();
      TOGGLE();
      TOGGLE();
      TOGGLE();

    }
    if ((PINE & M(WR)) == 0) {
      TOGGLE();
      TOGGLE();
    }
    TOGGLE();
    resetPin(BUSRQ);  //PORT_BUSRQ &= ~(BUSRQ_Pin);
    resetPin(WAIT_RES); //PORT_WAIT_RES &= ~WAIT_RES_Pin;  //reset RS-FF
    while (PIND & M(BUSACK))
      ;
    setPin(WAIT_RES); //PORT_WAIT_RES |= WAIT_RES_Pin;
    setPin(BUSRQ);  //PORT_BUSRQ |= BUSRQ_Pin;
  // restart
  }
}


//
// setup, loop
//
void setup() {
  // put your setup code here, to run once:
  //
  // Pin Mode
  //
  PORTG = 0b00000000;
  DDRG |= 0b00000011;
  TOGGLE();
  TOGGLE();
  //
  // init peripherals
  //
  UART_init();
  //
  // Port initialize
  //
  PORTD |= M(NMI);
  DDRD |=  M(NMI);
  PORTE |= M(BOOT)|M(BUSRQ)|M(WAIT_RES);
  DDRE |=  M(BOOT)|M(BUSRQ)|M(WAIT_RES);
  PORTB |= M(INT); // CLK, RESET remains zero
  PORTB |= 0b00000111;      // for SPI pins
  DDRB |=  M(INT)|M(RESET)|M(CLK);

  //
  // reset RS-FF
  //
  resetPin(WAIT_RES);
  setPin(WAIT_RES);
  //
  // CPU reset
  //  
  oneCLK(5);  

  //
  // clear SRAM
  //
  sram_begin();
  for (soff_t i = 0; i < 0x10; ++i) {
    sram_write(i, 0x0);
  }
  sram_write(0, 0xd3);  // IN A, (0)
  sram_write(1, 0);
  sram_write(2, 0xdb);  // OUT A,(0)
  sram_write(3, 0);
  sram_write(4, 0x76);  // HALT
  sram_end();

  
  initCLK();
  startCLK();

  //
  // start CPU
  //
  //setPin(RESET);

}

extern "C" int xmonitor(void);

void loop() {
  // put your main code here, to run repeatedly:
  //TOGGLE();
  //iop();
  xmonitor();
}
