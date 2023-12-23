#ifndef _SYS_DFU_H
#define _SYS_DFU_H

#include <stdbool.h>

typedef enum {
  dfu_flag_clear = 0x00000000,
  dfu_flag_present = 0xDEADBEEF,
} dfu_flag_t;

/**
 * @brief get the dfu flag
 *
 * @return dfu_flag_t flag
 */
dfu_flag_t dfu_get_flag();

/**
 * @brief set the dfu flag
 *
 * @param flag the flag
 */
void dfu_set_flag(dfu_flag_t flag);

/**
 * @brief jump to download mode
 *
 */
void dfu_jump_to_dlmode();

#endif /* _SYS_DFU_H */
