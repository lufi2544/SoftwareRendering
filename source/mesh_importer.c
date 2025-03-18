
///////// IMPORTER FOR MESH DATA ////////
// For now this is just a simple importer that reads a .obj file and parses that to mesh data.

enum importer_token_type
{
	token_end_of_stream,
	token_error,
	token_number,
	token_vertex_comp,
	token_face_comp,
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
					// advancing to the number
					start = ++at;
					
					// parse until next vertex components
					while(is_in_bounds(source, at) && source.bytes[at] != 'v')
					{						
						u8 val = source.bytes[at];
						// end of a number
						if(val == ' ' || val == '\n' || val == '\r' || val == '\t')
						{
							importer_element_t *vertex_element = PushStruct(_temp_arena, importer_element_t);
							vertex_element->next_sibling = 0;
							vertex_element->type = token_vertex_comp;
							vertex_element->value.bytes = source.bytes + start;
							vertex_element->value.size = at - start;	
							
							if(!first_element)
							{
								first_element = vertex_element;
							}
							
							if(element_ptr)
							{
								element_ptr->next_sibling = vertex_element;
							}
							
							element_ptr = vertex_element;
							start = ++at;
						}
						else
						{
							at++;							
						}						
					}																																												
				}
				
			}break;
			
			case 'f':
			{			
				u64 start = at;
				
				// parse until the next face which is f in the file.
				if(is_in_bounds(source, at) && (source.bytes[at] == ' '))
				{				
					start = ++at;
					
					while(is_in_bounds(source, at) && source.bytes[at] != 'f')
					{
						u8 val = source.bytes[at];
						// end of a number
						if(val == ' ' || val == '\n' || val == '\r' || val == '\t')
						{
							importer_element_t *face_element = PushStruct(_temp_arena, importer_element_t);
							face_element->next_sibling = 0;
							face_element->type = token_face_comp;
							face_element->value.bytes = source.bytes + start;
							face_element->value.size = at - start;	
							
							if(!first_element)
							{
								first_element = face_element;
							}
							
							if(element_ptr)
							{
								element_ptr->next_sibling = face_element;
							}
							
							element_ptr = face_element;
							start = ++at;							
						}
						
						else
						{
							at++;							
						}						
					}										
					
				}break;
				
				default:
				{
				}break;
			}
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


internal f64 
convert_to_sign(buffer_t _source, u64 *_at_result)
{
	u64 at = *_at_result;
	
	f64 result = 1.0;
	if(is_in_bounds(_source, at) && (_source.bytes[at] == '-'))
	{
		result = -1.0;
		++at;
	}
	
	*_at_result = at;		
	return result;
}

internal f64 
convert_to_number(buffer_t _source, u64 *_at_result)
{
	u64 at = *_at_result;
	
	f64 result = 0.0f;
	while(is_in_bounds(_source, at))	
	{
		u8 value = _source.bytes[at] - (u8)'0';
		if(value < 10)
		{
			result = 10.0 * result + (f64)value;
			++at;
		}
		else
		{
			break;
		}
	}
	
	*_at_result = at;
	
	
	return result;
}

internal f64
convert_to_f64(buffer_t _source)
{
	f64 result = 0.0;
	
	buffer_t source = _source;
	u64 at = 0;
	
	f64 sign = convert_to_sign(source, &at);
	f64 number = convert_to_number(source, &at);
	
	if(is_in_bounds(source, at) && source.bytes[at] == '.')
	{
		++at;
		f64 C = 1.0 / 10.0;
		while(is_in_bounds(source, at))
		{
			u8 value = source.bytes[at] - (u8)'0';
			if(value < 10)
			{
				number += (C * (f64)value);
				C *= 1.0/ 10.0;
				++at;
			}
			else				
			{
				break;
			}
		}
		
		result = sign * number;
	}	
	
	return result;
}

internal vec3_t 
convert_to_vertex(importer_element_t *first_vertex_component)
{
	vec3_t result;
	result.x = 0;
	result.y = 0;
	result.z = 0;	
		
	
	f64 x = convert_to_f64(first_vertex_component->value);
	f64 y = convert_to_f64(first_vertex_component->next_sibling->value);
	f64 z = convert_to_f64(first_vertex_component->next_sibling->next_sibling->value);
	
	
	result.x = (f32)x;
	result.y = (f32)y;
	result.z = (f32)z;
	
	return result;
}

internal face_t
convert_to_face(importer_element_t *first_face_element)
{
	// each face element has 3 components a, b and c, which the a is the only element that we care about for now
	// v1/vt1/vn1
	face_t result;
	
	
	// Let's make this an array moduled
	u64 at = 0;
	result.a = (s32)convert_to_number(first_face_element->value, &at) - 1;	
	
	at = 0;
	result.b = (s32)convert_to_number(first_face_element->next_sibling->value, &at) - 1;
	
	at = 0;
	result.c = (s32)convert_to_number(first_face_element->next_sibling->next_sibling->value, &at) - 1;
	
	//printf("face: a: %i, b: %i, c: %i \n", result.a, result.b, result.c);
	
	return result;
}


// For now let's make this simple, so we have an array of structs,
// I would like to change this to struct of arrays, so we can have id DOD and better for cache locality
// in this case, maybe allocting everything in the temp memory and then passing it to the permanent memory?..
internal mesh_t
create_mesh_from_file(engine_memory_t *engine_memory, const char *_file_name)
{
	mesh_t result;
	result.face_num = 0;
	result.vertex_num = 0;
	result.rotation.x = 0;
	result.rotation.y = 0;
	result.rotation.z = 0;
	
	if (_file_name == 0)
	{
		return result;
	}	
	
	TEMP_MEMORY();				
	
	buffer_t buffer = read_file(temp_memory.arena, _file_name);
	if (buffer.size > 0)
	{						
		printf("Importing mesh %s \n", _file_name);
		mesh_importer_t importer;
		importer.source = buffer;
		importer.at = 0;
		importer.had_error = false;		
		
		importer_element_t *element = import_mesh(temp_memory.arena, &importer);
		
		s32 count_face = 0;
		s32 count_vertex = 0;
		s32 iterated = 0;
		
		
		// convert to while and advace 3;
		// note:(juanes.rayo): add the same thing for the faces.
		for(importer_element_t *it = element; it; it = it->next_sibling)
		{
		
			iterated++;
			if(it->type == token_face_comp)
			{
				result.face_num++;
			}
			else if(it->type == token_vertex_comp)
			{
				result.vertex_num++;
			}
		}
		
		result.vertex_num /= 3;
		result.face_num /= 3;
		printf("iterated ver: %i \n", result.vertex_num);
		printf("iterated face: %i \n", result.face_num);		
		
		result.verteces = PushArray(&engine_memory->permanent, result.vertex_num, vec3_t);
		result.faces = PushArray(&engine_memory->permanent, result.face_num, face_t);
		
		//(juanes.rayo) NOTE: can we figure this out runtime? or is better to store it too? as we have already the verteces and the faces, we could figure this out runtime.
		//result.triangles = PushArray(&engine_memory->permanent, count_face, triangle_t);
		
		u32 current_vertex = 0;
		u32 current_face = 0;				
		
		f64 temp_vertex_sign = 1.0f;
		
		// Face and Vertex fill
				
		importer_element_t *it = element;		
		while(it != 0)
		{			
			if(it->type == token_face_comp)
			{
				face_t face = convert_to_face(it);								
				result.faces[current_face++] = face;
				it = it->next_sibling->next_sibling->next_sibling;
			}
			else if(it->type == token_vertex_comp)
			{								
				vec3_t vertex = convert_to_vertex(it);				
				
				result.verteces[current_vertex] = vertex;
				current_vertex++;
				
				it = it->next_sibling->next_sibling->next_sibling;
			}
		}
		
		// Checking quantity equality between the parsed faces and vertex num and the ones pushed to the mesh_t.
		
		//printf("parsed: %i result %i \n", current_face, result.face_num );
		
		assert(current_face == result.face_num);
		assert(current_vertex == result.vertex_num);				
	}
	
	
	
	TEMP_MEMORY_END();
			

	return result;
}
