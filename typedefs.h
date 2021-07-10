//
// typedefs.h ... integrate subsystem typedefs
//

#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <inttypes.h>

// a type, compatible with 'char' as well as it it used in 'sram[]'
// Strict signedness discrimination is needed where in alithmetic operations,
// including sumcheck, crc16 calucuration.
typedef uint8_t byte_t;

// sram address/offset
typedef uint32_t soff_t;

// serial flash address type
// usually 24bit-width (less than 16MB), or some more for extended address
//
typedef int32_t faddr_t;
typedef int32_t foff_t;

// generil pointer/integer type,
union _arg_t {
    uint32_t i;
    byte_t *bp;
};
typedef union _arg_t arg_t;

#endif //__TYPEDEFS_H
