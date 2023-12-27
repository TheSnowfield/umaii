#ifndef _CONFIG_H
#define _CONFIG_H

#define PNP_NORMAL_CLOSED 0
#define PNP_NORMAL_OPEN   1
#define NPN_NORMAL_CLOSED 2
#define NPN_NORMAL_OPEN   3

/**
 * @brief specific log format buffer size
 */
#ifndef UMAIIDX_LOG_FMTSIZE
  #define UMAIIDX_LOG_FMTSIZE 256
#endif

/**
 * @brief photo interrupter type
 */
#ifndef UMAIIDX_PHOTO_INTERRUPTER
  #define UMAIIDX_PHOTO_INTERRUPTER NPN_NORMAL_OPEN
#endif

/**
 * @brief enable serial log print
 * this option is CONFLICT with game button(over serial)
 */
#define UMAIIDX_ENABLE_LOGPRINT
#ifdef UMAIIDX_ENABLE_LOGPRINT
  #define UMAIIDX_LOGPRINT_BAUDRATE 921600
#endif

/**
 * @brief enable io4 emulation
 * all button input operations will be transmitted via usb hid.
 */
#ifndef UMAIIDX_ENABLE_IO4
  #define UMAIIDX_ENABLE_IO4 0
#endif

/**
 * @brief touch panel serial baudrate
 * the default value is 9600. higher baud rate causes
 * low transmission time and reduces the latency.
 *
 * maybe segatool supports a custom baud rate? idk
 */
#ifndef UMAIIDX_TOUCHPANEL_BAUDRATE
  #define UMAIIDX_TOUCHPANEL_BAUDRATE 9600
#endif


#endif /* _CONFIG_H */
