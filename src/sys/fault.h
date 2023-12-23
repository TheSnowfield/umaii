#ifndef _SYS_FAULT_H
#define _SYS_FAULT_H

#include <stdint.h>
#include <sys/core.h>

/**
 * @brief stop the mcu, disables all interrupts
 */
void panic();

void fault();
void fault_ex(status_t reason);
void assert(uint8_t condition, status_t reason);
status_t get_fault_reason();

#endif /* _SYS_FAULT_H */
