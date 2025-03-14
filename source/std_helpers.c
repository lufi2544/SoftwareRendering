

u32 cstr_len(const char* _str)
{	
	if(_str == 0)
	{
		return 0;
	}
	
	u32 size = 0;
	char ptr = ' ';
	while(ptr != '\0')
	{
		ptr = _str[size];	
		++size;
	}
	
	return size - 1;
}