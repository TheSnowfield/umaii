#ifndef _APP_MISC_LOGPRINT_H
#define _APP_MISC_LOGPRINT_H

#include <stddef.h>

/**
 * @brief log print init
 */
void logprint_init();

/**
 * @brief print log string
 *
 * @param str string
 */
void logprint_print(const char* str);

/**
 * @brief print log string advanced
 *
 * @param str string
 * @param size the length of the string
 */
void logprint_print_ex(const char* str, size_t size);

#endif /* _APP_MISC_LOGPRINT_H */
