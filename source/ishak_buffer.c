
internal_f b32 
is_in_bounds(buffer_t _buffer, u64 _at)
{
	b32 result = (_at < _buffer.size);
	return result;
}

internal_f buffer_t
create_buffer(arena_t *_arena, u64 _size)
{
	buffer_t result = {0, 0};
	if(!_size)
	{
		return result;
	}	
	
	void* allocated_data  = push_size(_arena, _size);
	if(allocated_data == 0)
	{
		return result;
	}
	
	result.bytes = allocated_data;
	result.size = _size;
	
	return result;
}

internal_f buffer_t 
create_buffer_string(arena_t *_arena, u32 _size)
{
	buffer_t result = create_buffer(_arena, _size);
	u8 *bytes_as_u8 = (u8*)result.bytes;
	bytes_as_u8[result.size - 1] = '\0';
	
	return result;
}

internal_f bool 
buffer_is_equal(buffer_t a, buffer_t b)
{
	if(a.size != b.size)
	{
		return false;
	}
	
	for(u64 i = 0; i < a.size; ++i)
	{
		if(a.bytes[i] != b.bytes[i])
		{
			return false;
		}
	}
	
	return true;
}



internal_f bool 
buffer_is_equal_cstring(buffer_t a, const char* b )
{		
	if((cstr_len(b) + 1) != a.size)
	{
		return false;
	}
		
	for(u64 i = 0; i < a.size; ++i)
	{
		if(a.bytes[i] != b[i])
		{
			return false;
		}
	}
	
	return true;
}

// With memory arena this is not needed.
internal_f void 
free_buffer(buffer_t *_buffer)
{
	if(_buffer->bytes)
	{		
		_buffer->bytes = 0;
	}	
}

