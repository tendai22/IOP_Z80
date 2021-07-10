//
// uart.h
//

#ifdef USE_UART
#ifndef __UART_H
#define __UART_H
#if defined(__cplusplus)
extern "C" {
#endif

extern void UART_init(void);
extern int xgetch_timeout(int32_t timeout);
extern void xputch(uint8_t c);
extern int xgetch_pol(void);
extern int xprintf(const char *format, ...);

#if defined(__cplusplus)
}
#endif //__cplusplus
#endif //__UART_H
#endif //USE_UART
