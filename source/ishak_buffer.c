
////////// BUFFER //////////

// This acts as a memory container of data.

typedef struct
{
	void* bytes;
	u64 size;
		
} buffer_t;


internal buffer_t
create_buffer(memory_arena_t *_arena, u64 _size)
{
	buffer_t result;
	if(!_size)
	{
		return result;
	}	
	
	// If we are in Windows maybe changing this to VAlloc? make this platform depenant
	void* allocated_data  = PushSize(_arena, _size);
	if(!allocated_data)
	{
		return result;
	}
	
	result.bytes = allocated_data;
	result.size = _size;
	
	return result;
}

// With memory arena this is not needed.
internal void 
free_buffer(buffer_t *_buffer)
{
	if(_buffer->bytes)
	{		
		_buffer->bytes = 0;
	}	
}

