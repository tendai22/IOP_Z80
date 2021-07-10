//
// monitor_lite.c
//
#include "config.h"
#ifdef USE_MONITOR
#if defined(__cplusplus)
extern "C" {
#endif

//
// intel hex format reader
//
int unget_buf = -1;

//
// machine dependept functions
//
int getch (void)
{
  int c;
  if (unget_buf >= 0) {
    c = unget_buf;
    unget_buf = -1;
    return c;
  }
  while ((c = xgetch_timeout(500L)) == -1)
    ;
  return c;
}

// one character read, with the specified timeout
// for xmodem receive function.
int getch_timeout (int32_t msec)
{
  int c;
  if (unget_buf >= 0) {
    c = unget_buf;
    unget_buf = -1;
    return c;
  }
  c = xgetch_timeout (msec);
  return c;
}

int ungetch (byte_t c)
{
  unget_buf = c;
  return c;
}

void crlf (void)
{
  xprintf ("\r\n");
}

//
// readline
//
int readline (char *buf, int len)
{
  char *p = buf;
  int c;
  while (p < (buf + len - 1) && (c = getch()) != '\r' && c != 'n') {
    if (c == 0x08 || c == 0x7f) {
      if (p > buf) {
        xprintf ("\x08 \x08");
        --p;
      }
    } else {
      xputch (c);
      *p++ = c;
    }
  }
  *p = '\0';
  return p - buf;
}

// convert to capital letters
static void strtoupper (char *dest, const char *src, int len)
{
  char *end = dest + len - 1;
  for (; *src; src++, dest++) {
    if (dest >= end) {
      *end = '\0';
      return;
    }
    *dest = toupper (*src);
  }
  *dest = '\0';
  return;
}

int strnccmp (const char *s1, const char *s2)
{
  char buf1[80], buf2[80];
  strtoupper (buf1, s1, sizeof buf1);
  strtoupper (buf2, s2, sizeof buf2);
  return strcmp ((const char *)buf1, (const char *)buf2);
}

//
// monitor main loop
//
int xmonitor (void)
{
  static char linbuf[40];
  //int continue_flag = 0;
  int c;
  int run_mode = 0;

  xprintf ("IOP_Z80 monitor\r\n");
  do {
    if (run_mode == 0) {
      xprintf ("> ");
    }
    c = getch ();
    if (c != ' ') {
      if (run_mode != 0) {
        xprintf ("> ");
      }
      run_mode = 0;
    }
    if (c == 'i') {
      xputch (c);
      // generate INT (i8251 RxRdy emulation)
      //generate_i8251int ();   // GPIOA->ODR |= INT_Pin;
    } else if (c == 'g' || c == ' ') {
      if (run_mode == 0) {
        xputch (c);
        xprintf ("\r\n");
        run_mode = 1;
      }
      // start CPU, nonstop mode
      // SPC ... singlestep, g ... run (nonstop)
      while (xgetch_pol() > 0);  // read out unread chars
      //runF3850_EXTI ((c == ' ') ? 1 : 0, continue_flag, &context);
      //continue_flag = 1;
    } else if (c == ':') {
      // intel hex format read
      long total;
      soff_t begin = UINT16_MAX, end = 0;
      uint16_t crc_val;
      uint32_t csum_val;
      int i;
      long n;
      ungetch (c);
      total = 0;
      if ((n = do_intelhex_receive (0, UINT16_MAX, &begin, &end)) > 0) {
        total += n;
        xprintf ("begin: %04lX, end: %04lX, len = %ld(%lX)\r\n", begin, end, (end - begin), (end - begin));
        crc_val = crc16_ccitt (begin, end, &sram_read);
        csum_val = 0;
        for (i = begin; i < end; ++i) {
          csum_val += sram_read(i);
        }
        xprintf ("crc16; %X, csum: %X\r\n", (unsigned int)crc_val, (unsigned int)csum_val);
      }
    } else {
      // gather chars in a line
      ungetch (c);
      readline (linbuf, sizeof linbuf);
      xprintf ("\r\n");
      //do_command (linbuf, cmdtable, NCMDS(cmdtable));
    }
  } while (1);
  return c;
}

#if defined(__cplusplus)
}
#endif //__cplusplus
#endif //USE_MONITOR