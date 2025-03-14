/* date = February 21st 2025 10:14 am */

////////////////// Global  for the Engine ///////////////

#include "ishak_buffer.h"
#include "ishak_string.h"
#include "ishak_arena.h"
#include "memory.h"

global bool is_running = true;
global engine_memory_t g_engine_memory;

#define TEMP_MEMORY() \
temp_memory_t temp_memory = temp_memory_init(&g_engine_memory.transient);    \
memory_arena_t* temp_arena = temp_memory.arena; \

#define TEMP_MEMORY_END() temp_memory_end(temp_memory);