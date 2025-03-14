
///////// IMPORTER FOR MESH DATA ////////
// For now this is just a simple importer that reads a .obj file and parses that to mesh data.

enum importer_token_type
{
	token_end_of_stream,
	token_error,
	token_number,	
	token_vertex,
	token_face,
	token_null,
	
	token_count,
};

typedef struct
{
	enum importer_token_type type;
	buffer_t value;
	
} importer_token_t;

typedef struct importer_element_t
{
	enum importer_token_type type;
	buffer_t value;	
    struct importer_element_t *next_sibling;
	
} importer_element_t;

typedef struct
{
	buffer_t source;
	u64 at;
	b32 had_error;
	
} mesh_importer_t;

internal b32 
is_white_space(buffer_t _source, u64 _at)
{
	b32 result = false;	
	if(is_in_bounds(_source, _at))
	{
		u8 val = _source.bytes[_at];
		result = ((val == ' ') || (val == '\t') || (val == '\n') || (val == '\r'));
	}
	
	return result;
}

internal b32
is_digit(buffer_t _source, u64 _at)
{
	b32 result = false;
	if(is_in_bounds(_source, _at))
	{
		u8 val = _source.bytes[_at];
		result = ((val >= '0') && (val <= '9'));
	}
	
	return result;
}

internal importer_element_t*
import_mesh(memory_arena_t *_temp_arena, const mesh_importer_t *_importer)
{	
	buffer_t source = _importer->source;
	u64 at = _importer->at;
	importer_element_t *first_element = 0;
	importer_element_t *element_ptr = 0;
	
	
	while(is_in_bounds(source, at))
	{		
		
		// Skiping white spaces
		while(is_white_space(source, at))
		{
			++at;
		}
		
		u8 val = source.bytes[at++];
		
		// we move the parsing pointer until we reach something significant and save the buffer.
		switch(val)
		{					
			case 'v':
			{
				// potential vertex and other data			
				u64 start = at;
				if(is_in_bounds(source, at) && source.bytes[at] == ' ')
				{
					start = at;
					
					// parse until next vertex
					while(is_in_bounds(source, at) && source.bytes[at + 1] != 'v')
					{
						at++;
					}									
					
					
					importer_element_t *vertex_element = PushStruct(_temp_arena, importer_element_t);
					vertex_element->next_sibling = 0;
					vertex_element->type = token_vertex;
					vertex_element->value.bytes = source.bytes + start;
					vertex_element->value.size = at - start;
					
					//printf("adding a vertex \n");
					
					if(!first_element)
					{
						first_element = vertex_element;
					}
					
					if(element_ptr)
					{
						element_ptr->next_sibling = vertex_element;
					}
					
					element_ptr = vertex_element;
										
				}
				
			}break;
			
			case 'f':
			{			
				u64 start = at;
				
				// parse until the next face which is f in the file.
				if(is_in_bounds(source, at) && (source.bytes[at] == ' '))
				{				
					start = at;
					while(is_in_bounds(source, at) && source.bytes[at + 1] != 'f')
					{
						at++;
					}										
					
					
					importer_element_t *face_element = PushStruct(_temp_arena, importer_element_t);
					face_element->next_sibling = 0;
					face_element->type = token_face;
					face_element->value.bytes = source.bytes + start;
					face_element->value.size = at - start;
					
					//printf("adding a face \n");
					
					if(!first_element)
					{
						first_element = face_element;
					}
					
					if(element_ptr)
					{
						element_ptr->next_sibling = face_element;
					}
					
					element_ptr = face_element;										
				}
				
			}break;
			
			default:
			{
			}break;
		}
	}
	
	
	return first_element;
}


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
	printf("Importing mesh %s \n", _file_name);
	mesh_t result;
	
	if (_file_name == 0)
	{
		return result;
	}	
	
	temp_memory_t temp_memory = temp_memory_init(&engine_memory->transient);
	
	buffer_t buffer = read_file(temp_memory.arena, _file_name);
	if (buffer.bytes)
	{						
		mesh_importer_t importer;
		importer.source = buffer;
		importer.at = 0;
		importer.had_error = false;
		
		
		importer_element_t *element = import_mesh(temp_memory.arena, &importer);
		
		s32 count_face = 0;
		s32 count_vertex = 0;
		s32 iterated = 0;
		
		for(importer_element_t *it = element; it; it = it->next_sibling)
		{
			iterated++;
			if(it->type == token_face)
			{
				count_face++;
			}
			else if(it->type == token_vertex)
			{
				count_vertex++;
			}
		}
				
		
	}
	
	temp_memory_end(temp_memory);
	return result;
}
