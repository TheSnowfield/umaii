#ifndef _SYS_DEBUG_H
#define _SYS_DEBUG_H

#include <stddef.h>
#include <stdarg.h>

typedef enum {
  loglevel_debug   = 0,
  loglevel_info    = 1,
  loglevel_warning = 2,
  loglevel_error   = 3,
  loglevel_fatal   = 4,
  loglevel_max
} loglevel_t;

#ifdef _MSC_VER
  #define TAG __FUNCTION__
#elif __GNUC__
  #define TAG __func__
#else
  #warning "Your compiler may not supported __func__ macro, so defined the TAG as '---'"
  #define TAG "---"
#endif

typedef void (* log_print_cb_t)(loglevel_t level, const char* tag, const char* fmt, va_list args);

/**
 * @brief log debug
 *
 * @param tag tag
 * @param fmt format string
 * @param ... string arguments
 */
void log_debug(const char* tag, const char* fmt, ...);

/**
 * @brief log information
 *
 * @param tag tag
 * @param fmt format string
 * @param ... string arguments
 */
void log_info(const char* tag, const char* fmt, ...);

/**
 * @brief log warning
 *
 * @param tag tag
 * @param fmt format string
 * @param ... string arguments
 */
void log_warning(const char* tag, const char* fmt, ...);

/**
 * @brief log error
 *
 * @param tag tag
 * @param fmt format string
 * @param ... string arguments
 */
void log_error(const char* tag, const char* fmt, ...);

/**
 * @brief log fatal
 *
 * @param tag tag
 * @param fmt format string
 * @param ... string arguments
 */
void log_fatal(const char* tag, const char* fmt, ...);

/**
 * @brief hexdump
 * @param tag tag
 * @param buf buffer
 * @param len buffer length
*/
void hexdump(const char* tag, const unsigned char* buf, size_t len);

/**
 * @brief set log callback
 *
 * @param callback
 */
void log_set_cb(log_print_cb_t callback);

#endif /* _SYS_DEBUG_H */
