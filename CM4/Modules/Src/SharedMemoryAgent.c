/*
 * SharedMemoryAgent.c
 *
 *  Created on: Jan 19, 2025
 *      Author: nadavguy
 */
//#include "Shared_memory.h"
#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "Shared_memory.h"

void read_data_from_m7(void)
{
    // Wait for HSEM notification
    while (HAL_HSEM_IsSemTaken(0) == 1); // Busy-wait (or use interrupt)

    uint32_t data = shared_data;  // Read data from shared memory
}
