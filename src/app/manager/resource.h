#ifndef _APP_MANAGER_RESOURCE_H
#define _APP_MANAGER_RESOURCE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


/**
 * @brief resource initialization
 * @param address flash address
 */
bool resource_init(uint32_t address);

/**
 * @brief uninit
 */
void resource_uninit();

/**
 * @brief fetch resources
 * @param path path to the file
 * @return if success return file data
 */
const uint8_t* resource_fetch(const char* path);

#endif /* _APP_MANAGER_RESOURCE_H */
