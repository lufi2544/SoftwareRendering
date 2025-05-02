
////////// SIMPLE FILE READER /////////

#include "sys/stat.h"

internal_f buffer_t
read_file(arena_t *_arena, const char *_file_name)
{
	buffer_t result;
	result.size = 0;
	result.data = 0;
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
	struct _stat64 stat;
	s32 stat_result = _stat64(_file_name, &stat);
#else
	struct stat Stat;
	s32 stat_result = stat(_file_name, &stat);
#endif        
	
	if(stat_result != 0)
	{
		return result;
	}	
	
	result = create_buffer(_arena, sizeof(u8) * stat.st_size); // reduntand but well..		
	u64 read_elements = fread(result.data, sizeof(u8), result.size, file);
	if(read_elements == 0)
	{
		// successfully read the file do stuff
		fprintf(stderr, "Unable to read the file %s \n", _file_name);		
		return result;
	}
	
	
	return result;
}

