/*
 * monitor.h
 *
 *  Created on: Oct 19, 2020
 *      Author: tenda_000
 */

#ifndef SRC_MONITOR_H_
#define SRC_MONITOR_H_
#if defined(__cplusplus)
extern "C" {
#endif

#include <limits.h>
#include "config.h"
#include "typedefs.h"

#if defined(USE_MONITOR)
//
// pointer-in-monitor
//
// Its storage size is enough (or just as) to accommodate a generic pointer,
// If its value is within 0x0000 - 0xffff, it represents an offset in sram[].
//
// address within 0000 - FFFF regards as in sram[offset]
//

// SRAM area
//extern byte_t *sram;
int xprintf (const char *format, ...);

#if !defined(SRAM_SIZE)
#define SRAM_SIZE 0x10000
#endif

#ifdef LINUX
//#define BADADDR ULONG_MAX
//#define PHYADDR(p) (sram + p)
#else
extern byte_t __not_used;
#define BADADDR (0xffffffff)
#define PHYADDR(p) (p)
#endif

#if 0
//
// target CPU execution context
//
struct codeinfo {
	byte_t *code;
	uint16_t start;
	int length;
};
#endif

extern struct codeinfo codes[];

//
// command table and its entry
//
struct cmd_entry {
	char *format;
	int (*func)(int ac, arg_t *av);
};

//
// global function declarations
//
//int xmonitor (void);
int xgetch (void);
int ungetch (byte_t c);
int xgetch_timeout (int32_t millisec);
void crlf (void);
int readline (char *buf, int len);
int strnccmp (const char *a, const char *b);
int do_command (const char *cmdstr, const struct cmd_entry *cmdtable, int ncmds);

#endif //USE_MONITOR

// trailing magic spells
#if defined(__cplusplus)
}
#endif
#endif /* SRC_MONITOR_H_ */
