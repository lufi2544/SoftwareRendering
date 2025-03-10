////////// MEMORY ARENA ////////

// This arena aims to use the memory in a way of a Z++ programmer, so we basically see everything as a hole pack of bytes,
// so we allocate everything at the same time and deallocate everything also at the same time.


typedef u32 memory_index;

typedef struct
{
	u8* base;
	u64 size;
	memory_index used;
	s32 temp_count;
	
} memory_arena_t;


typedef struct
{
	memory_arena_t* arena;
	memory_index used;
	
} temp_memory_t;


internal void
initialize_arena(memory_arena_t *_arena, memory_index _size, void *_base)
{
	_arena->size = _size;
	_arena->base = (u8*)_base;
	_arena->used = 0;
	_arena->temp_count = 0;
}

#define PushStruct(a, t) (t*)PushSize_(a, sizeof(t))
#define PushArray(a, c, t) (t*)PushSize_(a, (c) * sizeof(t))
#define PushSize(a, s) PushSize_(a, s)

internal void* 
PushSize_(memory_arena_t *_arena, memory_index _size)
{
	if(_arena->used + _size > _arena->size)
	{
		fprintf(stderr, "arena with size %i trying to add another %llu bytes. No enough space... \n", _arena->size, _size);
		assert(false);
	}
	
	void* result = _arena->base + _arena->used;
	_arena->used += _size;
	
	return result;
}

internal temp_memory_t
temp_memory_init(memory_arena_t *_arena)
{
	temp_memory_t result;
	
	result.arena = _arena;
	result.used = _arena->used;
	++_arena->temp_count;
	
	return result;
	
}


internal void 
temp_memory_end(temp_memory_t _temp)
{
	memory_arena_t *arena = _temp.arena;
	
	assert(arena->used >= _temp.used);
	arena->used = _temp.used;
	assert(arena->temp_count > 0);
	--arena->temp_count;
}