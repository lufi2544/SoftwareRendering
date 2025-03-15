/* date = March 10th 2025 9:19 am */

#define Kilobyte(v) v * 1024
#define Megabyte(v) Kilobyte(v) * 1024
#define Gigabyte(v) Megabyte(v) * 1024
#define MemoryPage(v) v * Kilobyte(4096) // This is platform dependant I think, so maybe chaning that for macos and linux.

#define ENGINE_MEMORY_TRANSIENT_SIZE Megabyte(500)
#define ENGINE_MEMORY_PERMANENT_SIZE Gigabyte(1)




typedef struct
{	
	memory_arena_t permanent;
	memory_arena_t transient;
	
} engine_memory_t;


#define TEMP_MEMORY() \
temp_memory_t temp_memory = temp_memory_init(&g_engine_memory.transient);    \
memory_arena_t* temp_arena = temp_memory.arena; \

#define TEMP_MEMORY_END() temp_memory_end(temp_memory);