#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
#include <stdint.h>
// Place the shared variable in the .shared_ram section
__attribute__((section(".shared_ram"))) volatile uint32_t shared_data;

#endif // SHARED_MEMORY_H
