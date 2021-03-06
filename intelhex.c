/*
 * intelhex.c
 *
 *  Created on: 2021/02/01
 *      Author: tenda_000
 */
#include "config.h"
#ifdef USE_UART
#include <stdio.h>
#include <string.h>
//#include "monitor.h"
//#include "machinedep.h"
//#include "intelhex.h"

//
// faster Intel HEX format reader
//

static uint8_t conv_table[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,

	0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//
// Intel Hex Reader
//
#define GETCH(__c,__num) do { if ((__c = xgetch_timeout(INTELHEX_TIMEOUT)) < 0) {\
	printf ("timeout error %d\r\n", __num); return -3; } } while(0)
#define GETBYTE(__x,__num) do { int __c; GETCH(__c,__num+1); __x = (conv_table[__c]<<4) & 0xf0; \
	GETCH(__c,__num+2); __x |= conv_table[__c]; } while(0)

#define INTELHEX_TIMEOUT 100
#define BUFLEN 80

int do_intelhex_receive (soff_t topram, soff_t endram, soff_t *beginp, soff_t *endp)
{
	byte_t buf[BUFLEN], *endbuf = &buf[BUFLEN];
	register byte_t *p;
	soff_t addr;
	int c, n, len, eodflag, sum, total, type;
	// top of line
	total = 0;
	eodflag = 0;
	while (eodflag == 0) {
		// read out until ':' be found.
		do {
			GETCH(c,1);
		} while (c != ':');
		// top of line, do initialize
		p = &buf[0];
		// byte1: length
		GETBYTE(len,2);
		sum = len;
		// byte2,3: address
		GETBYTE(c,4);
		addr = (c<<8)&0xff00;
		sum += c;
		GETBYTE(c,6);
		addr |= (c&0xff);
		sum += c;
		// byte4: type
		GETBYTE(type,8);
		sum += type;
		// byte5..:read body
		//xprintf ("%d %04X %d: ", type, addr, len);
		if (type == 1) {
			eodflag = 1;
			n = len;
			while (n-- > 0) {
				GETBYTE(c,12);
				sum += c;
			}
		} else if (type == 0) {
			p = &buf[0];
			n = len;
			while (n-- > 0) {
				GETBYTE(c,10);
				sum += c;
				//xprintf (" %02x", c);
				if (p < endbuf) {
					*p++ = c;
				}
			}
		}
		// check sum
		GETBYTE(c,16);
		sum += c;
		sum &= 0xff;
		if (sum != 0) {
			xprintf (" sumcheck error %02x\r\n", sum);
			return -2;
		}
		xprintf ("%d", type);
		// data copy
		if (type == 0) {
			soff_t top = addr;
			if (addr + len > endram) {
				len = endram - addr;
			}
			for (int i = 0; i < len; ++i) {
				sram_write (addr + i, buf[i]);
			}
			//memcpy (top, &buf[0], len);
			total += len;
			// revise coverage range
			if (beginp && *beginp > addr) {
				*beginp = addr;
			}
			if (endp && *endp < addr + len) {
				*endp = addr + len;
			}
		}
	} // end of while
	// data read out
	xprintf ("X\r\n");
	while (xgetch_timeout(20) >= 0)
		;
	return total;
}

#endif //USE_UART
