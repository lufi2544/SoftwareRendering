

/* Creates a string.
* 1. _content is not empty, so string len will mach _contentlen.
* 2. _content is not empty neither _len, then internal buffer will be _content size + _len + 1.
* 3. _content is empty but string not _len, then internal buffer will be _len + 1.
* 4. _content and _len are empty, then DEFAULT_EMPTY_STRING_LEN + 1 will be internal buffer size;
*
*/


// string raw default len
#define STRING(a) make_string(a, DEFAULT_EMPTY_STRING_LEN, 0)
// string lenght 
#define STRING_L(a, l) make_string(a, l, 0)
// string verbal
#define STRING_V(a, c) make_string(a, 0, c)
// string verbal lenght, ready to be expanded
#define STRING_VL(a, l, c) make_string(a, l, c)


internal string_t
make_string(memory_arena_t *_arena, u32 _len, const char* _content)
{
	string_t result;
	u32 content_len = cstr_len(_content);
	
	u32 string_buffer_size = 0;
	if(content_len > 0)
	{
		if(_len > 0)
		{
			string_buffer_size = (_len > 0) ? (content_len + _len) : (content_len);
		}		
	}
	else
	{
		if(_len == 0)
		{
			_len = DEFAULT_EMPTY_STRING_LEN;
		}
		
		string_buffer_size = _len;
	}
	
	// null operator;
	string_buffer_size += 1;
	
	buffer_t buffer_str = create_buffer_string(_arena, string_buffer_size);
	
	result.buffer = buffer_str;
	result.len = 0;
	
	if(content_len > 0)
	{
		assert(content_len + 1 == buffer_str.size);
		for(u32 i = 0; i < content_len; ++i)
		{
			buffer_str.bytes[i] = _content[i];
		}		
		
		result.len = content_len;
	}	
	
	return result;
}



internal void
string_push_char(string_t *_str, u8 character)
{
	if(_str == 0)
	{
		return;
	}
	
	// if hit, then request more memory for this string buffer upon creation.
	assert((_str->len + 1) < (_str->buffer.size));
	
    _str->buffer.bytes[_str->len++] = character;
    _str->buffer.bytes[_str->len] = '\0';		
}

internal bool 
is_equal_cstr(string_t *_str, const char* b)
{
	
	return buffer_is_equal_cstring(_str->buffer, b);
}


