#include <stdio.h>
#include <stddef.h>

#include <sys/debug.h>
#include <usrcfg.h>

static log_print_cb_t _callback = NULL;

void log_set_cb(log_print_cb_t callback) {
  _callback = callback;
}

void log_debug(const char* tag, const char* fmt, ...) {
  va_list _args;
  va_start(_args, fmt);
  if(_callback) _callback(loglevel_debug, tag, fmt, _args);
  va_end(_args);
}

void log_info(const char* tag, const char* fmt, ...) {
  va_list _args;
  va_start(_args, fmt);
  if(_callback) _callback(loglevel_info, tag, fmt, _args);
  va_end(_args);
}

void log_warning(const char* tag, const char* fmt, ...) {
  va_list _args;
  va_start(_args, fmt);
  if(_callback) _callback(loglevel_warning, tag, fmt, _args);
  va_end(_args);
}

void log_error(const char* tag, const char* fmt, ...) {
  va_list _args;
  va_start(_args, fmt);
  if(_callback) _callback(loglevel_error, tag, fmt, _args);
  va_end(_args);
}

void log_fatal(const char* tag, const char* fmt, ...) {
  va_list _args;
  va_start(_args, fmt);
  if(_callback) _callback(loglevel_fatal, tag, fmt, _args);
  va_end(_args);
}

#define ch(x) ((x) >= '0' && (x) <= 'z' ? (x) : '.')

void hexdump(const char* tag, const unsigned char* buf, size_t len) {

  log_debug(tag, "Hexdump of [%p], length %zu", buf, len);

  // aligned with 8 bytes
  for(size_t i = 0; i + 8 <= len; i += 8) {
    log_debug(tag,
      "[%p] %02x %02x %02x %02x %02x %02x %02x %02x  %c%c%c%c%c%c%c%c",
      buf + i,
      buf[i + 0], buf[i + 1], buf[i + 2], buf[i + 3],
      buf[i + 4], buf[i + 5], buf[i + 6], buf[i + 7],
      ch(buf[i + 0]), ch(buf[i + 1]), ch(buf[i + 2]), ch(buf[i + 3]),
      ch(buf[i + 4]), ch(buf[i + 5]), ch(buf[i + 6]), ch(buf[i + 7])
    );
  }

  if(len % 8 != 0) {
    char _buf[64];
    char* _p = _buf;

    _p += sprintf(_p, "[%p] ", buf + len - (len % 8));

    // print the rest of the bytes
    for(size_t i = len - (len % 8); i < len; ++i) {
      _p += sprintf(_p, "%02x ", buf[i]);
    }

    // text align with 8 hex digits
    for(size_t i = 0; i < 8 - (len % 8); ++i) {
      _p += sprintf(_p, "   ");
    }
    
    _p += sprintf(_p, " ");

    for(size_t i = len - (len % 8); i < len; ++i) {
      _p += sprintf(_p, "%c", ch(buf[i + 0]));
    }

    log_debug(tag, "%s", _buf);
  }
}

#undef ch
