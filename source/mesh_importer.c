
///////// IMPORTER FOR MESH DATA ////////
// For now this is just a simple importer that reads a .obj file and parses that to mesh data.



internal mesh_t
create_mesh_from_file(engine_memory_t *engine_memory, const char *_file_name)
{
	mesh_t result;
	
	if (_file_name == 0)
	{
		return result;
	}
	
	temp_memory_t temp_memory = temp_memory_init(&engine_memory->transient);
	
	buffer_t buffer = read_file(temp_memory.arena, _file_name);
	if (!buffer.bytes)
	{
		return result;
	}
	
	
	// Lets figure out how meny verteces and triangles and then we will allocate the memory within the arena.
		
	char* temp_string = PushArray(temp_memory.arena, 20, char);	
	
	u32 verteces_num = 0;
	u32 faces_num = 0;
	
	u8* buffer_as_bytes = (u8*)buffer.bytes;
	for(u32 i = 0; i < buffer.size; ++i)
	{
		u8 character = *(buffer_as_bytes + i);
		switch(character)
		{
			case 'v':
			{
				++verteces_num;
			}
			break; 
			
			case 'f':
			{
				++faces_num;
			}
			break;
			
			default: 
			break;
		}
	}		
	temp_memory_end(temp_memory);
	
	// Create the verteces and faces
	
	return result;
}
