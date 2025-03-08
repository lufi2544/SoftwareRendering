
////////// BUFFER //////////

// This acts as a memory container of data.

typedef struct
{
	u8* bytes;
	u64 size;
		
} buffer_t;


internal buffer_t
create_buffer(u64 _size)
{
	buffer_t result;
	if(!_size)
	{
		return result;
	}	
	
	// If we are in Windows maybe changing this to VAlloc? make this platform depenant
	u8* allocated_data  = (u8*)(malloc(_size));
	if(!allocated_data)
	{
		return result;
	}
	
	result.bytes = allocated_data;
	result.size = _size;
	
	return result;
}

internal void 
free_buffer(buffer_t *_buffer)
{
	if(_buffer->bytes)
	{
		free(_buffer->bytes);
		_buffer->bytes = 0;
	}	
}

