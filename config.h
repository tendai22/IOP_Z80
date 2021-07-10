//
// config.h ... global macros for system configure
//

#ifndef __CONFIG_H
#define __CONFIG_H
#if defined(__cplusplus)
extern "C" {
#endif

//
// CPUTYPES
//
#define ATMEGA64
//#define STM32F405
//#define STM32H750

//
// ARDUINO libraries
//
//#define USE_ARDUINO_GPIO
//#define USE_ARDUINO_SERIAL

//
// feature selections
//
//#define USE_UART

//
// monitor program
//
//#define USE_MONITOR

//
// feature to read Intel HEX format files
//
//#define USE_INTELHEX

//
// xmodem file downloader
//
//#define USE_XMODEM

// 
// SRAM connected/controlled with GPIO
//
#define USE_EXTSRAM
//
// if no definition of 'USE_EXTSRAM', we allocate
// 64kB global memory for the array 'uint8_t sram[]'
#define SRAM_SIZE 65536

//
// SPI feature
// if it is defined, the following functions should
// be provided;
// spi_init,
// spi_xfer,
// spi_cs_reset,
// spi_cs_set.
//
//#define USE_SPI

//
// prepare all the definitions
//
#if defined(USE_EXTSRAM)
#if !defined(USE_SRAM)
#define USE_SRAM
#endif
#endif

#if defined(USE_MONITOR)
#if !defined(USE_UART)
#define USE_UART
#endif
#endif

#if defined(USE_INTELHEX)
#if !defined(USE_UART)
#define USE_UART
#endif
#endif

#if defined(USE_XMODEM)
#if !defined(USE_UART)
#define USE_UART
#endif
#endif

//
// automatic includion
//

#include "typedefs.h"

#if defined(USE_SRAM)
#include "sram.h"
#endif

#if defined(USE_MONITOR)
#include "monitor.h"
#endif

#if defined(USE_SPI)
#include "xspi.h"
#endif //USE_SPI

#if defined(USE_SPI)
#include "xspi.h"
#endif

#if defined(USE_INTELHEX)
#include "intelhex.h"
#endif

#if defined(USE_XMODEM)
#include "xmodem.h"
#endif

#if defined(USE_XMODEM)
#include "xmodem.h"
#endif


#if defined(__cplusplus)
}
#endif
#endif //__CONFIG_H
