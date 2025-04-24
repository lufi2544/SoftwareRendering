/* date = March 10th 2025 9:19 am */


#define SCRATCH() \
temp_memory_t temp_memory = temp_memory_init(&g_engine_memory.transient);    \
memory_arena_t* temp_arena = temp_memory.arena; \

#define SCRATCH_END() temp_memory_end(temp_memory);