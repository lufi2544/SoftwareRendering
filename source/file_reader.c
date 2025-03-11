
////////// SIMPLE FILE READER /////////

#include "sys/stat.h"

internal buffer_t
ReadFile(memory_arena_t *_arena, const char *_file_name)
{
	buffer_t result;
	if(!_arena || !_file_name)
	{
		// TOD LOG
		return result;
	}
	
	FILE* file = fopen(_file_name, "rb");
	if(!file)
	{
		fprintf(stderr, "Unable to open the file %s \n", _file_name);
		return result;
	}
		
#if _WIN32
	struct _stat64 Stat;
	_stat64(_file_name, &Stat);
#else
	struct stat Stat;
	stat(_file_name, &Stat);
#endif        
	
	result = create_buffer(_arena, sizeof(u8) * result.size); // reduntand but well..		
	u64 read_elements = fread(result.bytes, sizeof(u8), result.size, file);
	
	if(read_elements == 0)
	{
		// successfully read the file do stuff
		fprintf(stderr, "Unable to read the file %s \n", _file_name);
		free_buffer(&result);		
		return result;
	}
	
	
	return result;
}

