#include <stdio.h>
#include <string.h>

#include <config.h>
#include <sys/debug.h>
#include <sys/tick.h>
#include <hardware/common.h>
#include <board/led15093.h>

#include "logprint.h"

/**
 * @brief print log
 *
 * @param level log level
 * @param tag tag
 * @param fmt format string
 * @param args format args
 */
void __callback_logprint(loglevel_t level, const char* tag, const char* fmt, va_list args) {

  size_t _len;
  char _logbuf[UMAIIDX_LOG_FMTSIZE];
  static char _lv[] = {'D', 'I', 'W', 'E', 'F'};

  // tag
  _len = snprintf(_logbuf, UMAIIDX_LOG_FMTSIZE, "[%d] [%c] [%s] ", tick_get_time(), _lv[level], tag); {
    logprint_print_ex(_logbuf, _len);
  }

  // format log string
  _len = vsnprintf(_logbuf, UMAIIDX_LOG_FMTSIZE, fmt, args); {
    logprint_print_ex(_logbuf, _len);
  }

  logprint_print_ex("\r\n", 2);
}

void logprint_init() {
#ifdef UMAIIDX_ENABLE_LOGPRINT
  log_set_cb(&__callback_logprint);
#endif
}

void logprint_print(const char* str) {
  logprint_print_ex(str, strlen(str));
}

void logprint_print_ex(const char* str, size_t size) {
#ifdef UMAIIDX_ENABLE_LOGPRINT
  led15093_mux_uart_tx((uint8_t *)str, size);
#endif
}
