
///////// IMPORTER FOR MESH DATA ////////
// For now this is just a simple importer that reads a .obj file and parses that to mesh data.

enum enum_parsing
{
	category_none = 0,
	category_vertex = 1,
	category_face = 2,
};


// For now let's make this simple, so we have an array of structs,
// I would like to change this to struct of arrays, so we can have id DOD and better for cache locality
// in this case, maybe allocting everything in the temp memory and then passing it to the permanent memory?..
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
	
		
	// For now just 20 of everything as temp, of course this will change
	vec3_t *temp_vecteces = PushArray(&engine_memory->transient, 20, vec3_t); 
	face_t *temp_faces = PushArray(&engine_memory->transient, 20, face_t);
	triangle_t *temp_triangles = PushArray(&engine_memory->transient, 20, triangle_t);
		
	string_t temp_string = STRING(temp_memory.arena);
	
	u32 verteces_num = 0;
	u32 faces_num = 0;
	
	vec3_t temp_vertex;
	face_t temp_face;
	triangle_t temp_triangle;
	u8 temp_string_size = 0; 
	enum enum_parsing parsing_category = category_none;
	
	
	u32 start = 0;
	u32 end = 0;
	
	for(u32 i = 0; i < buffer.size; ++i)
	{
		u8 character = *(buffer.bytes + i);
		switch(character)
		{
			case 'v':			 			
			case 'n':
			case 't':
			case 'f': 			
			
			string_push_char(&temp_string, character);
			break;
			
			case ' ':
			if(temp_string_size > 0)
			{
				/*
				if(buffer_is_equal_cstring(temp_buffer, "v"))
				{
					
				}
				else if(buffer_is_equal_cstring(temp_buffer, "vn"))
				{
					//nothing for now
				}
				else if(buffer_is_equal_cstring(temp_buffer, "vt"))
				{
					//nothing for now
				}
				else if(buffer_is_equal_cstring(temp_buffer, "f"))
				{
					faces_num++;
				}
				
				temp_string_size = 0;
*/
			}
			
			break;
			
			default: 
			break;
		}
	}		
	temp_memory_end(temp_memory);
		
	
	// Create the verteces and faces
	vec3_t *mesh_verteces = PushArray(&engine_memory->permanent, verteces_num, vec3_t);
	face_t *mesh_faces = PushArray(&engine_memory->permanent, faces_num, face_t);
	
	
	
	return result;
}

vec3_t parse_vertex(buffer_t _buffer, u32 _start, u32 _end, memory_arena_t *_transient_arena)
{
	temp_memory_t temp_memory = temp_memory_init(_transient_arena);
	
	string_t temp_string = STRING(temp_memory.arena);
	u8 temp_string_size = 0;
	
	vec3_t result;
	
	// if _end == 0 this means we are at the begining of the file, then skip
	if(_end != 0)
	{
		for(u32 i = _start; i < _end; ++i)
		{
			u8 character = _buffer.bytes[i];
			if(character != ' ')
			{
			}
			else
			{
				
			}
		}
	}
	
	temp_memory_end(temp_memory);	
	return result;
}

