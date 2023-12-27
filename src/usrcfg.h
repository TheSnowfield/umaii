#ifndef _USRCFG_H
#define _USRCFG_H

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
 * @brief log print baudrate
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

#endif /* _USRCFG_H */
