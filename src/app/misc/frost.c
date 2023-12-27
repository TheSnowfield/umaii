#include <stdint.h>
#include <stdarg.h>

#include <sys/tick.h>
#include <sys/debug.h>

/**
 * @brief porting log print for FrostOS
 *
 * @param tag tag
 * @param fmt format
 */
void __frost_log_print(const char* tag, const char* fmt, ...) {
  va_list _args;
  va_start(_args, fmt);
  log_debug(tag, fmt, _args);
  va_end(_args);
}

/**
 * @brief porting time tick for FrostOS
 *
 * @param tick system tick
 */
uint64_t __frost_time_tick(uint64_t* tick) {
  uint64_t _tick = tick_get_time();
  if(tick) *tick = _tick;
  return _tick;
}
