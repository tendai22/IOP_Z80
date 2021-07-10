//
// uart for ATmega64
//
#include "config.h"
#if defined(USE_UART)
#include <stdarg.h>
#include <Arduino.h>

#if defined(__cplusplus)
extern "C" {
#endif

//
// Serial function
//
void UART_init(void)
{
#ifdef USE_AVRUART
  unsigned int ubrr = 21;  // 57600bps @ 20MHz
  ubrr = 10;      // 115200bos @ 20MHz
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;

  UCSR0B = (1<<RXEN0) | (1<<TXEN0);

  UCSR0C = (1<<USBS0) | (3<<UCSZ00);
#endif
#ifdef USE_ARDUIN_OSERIAL
  Serial.begin(57600);
#endif
}

int xgetch_timeout(int32_t timeout)
{
  uint32_t cur, start = millis();
  while ((cur = millis()) - start < timeout) {
#ifdef USE_ARDUINO_SERIAL
    if (Serial.available() > 0) {
      return Serial.read();
    }
#endif
#ifdef USE_AVRUART
    if ((UCSR0A & (1<<RXC0)) != 0) {
      return UDR0;
    }
#endif
    //delay(2);
  }
  return -1;
}

void xputch(uint8_t c)
{
#ifdef USE_ARDUINO_SERIAL
  Serial.write(c);
#endif
#ifdef USE_AVRUART
  while (!(UCSR0A & (1<<UDRE0)))
    ;
  UDR0 = c;
#endif
}

int xgetch_pol(void)
{
#ifdef USE_ARDUINOSERIAL
	if (Serial.available()) {
		return Serial.read();
	} else {
		return -1;
	}
#endif
#ifdef USE_AVRUART
  if ((UCSR0A & (1<<RXC0)))
    return UDR0;
  else
    return -1;
#endif
}
int xprintf(const char *format, ...)
{
  char *p, buf[80];
  //Serial.write("xxx\r\n");
  va_list ap;
  va_start (ap, format);
  vsnprintf (buf, sizeof buf, format, ap);
  va_end(ap);
#ifdef USE_AVRUART
  for (p = &buf[0]; p < &buf[80] && *p; ++p)
    xputch (*p);
#endif
#ifdef USE_ARDUINOSERIAL
  Serial.write(buf);
#endif
}
#if defined(__cplusplus)
}
#endif //__cplusplus
#endif //USE_UART