
///////// IMPORTER FOR MESH DATA ////////
// For now this is just a simple importer that reads a .obj file and parses that to mesh data.

enum importer_token_type
{
	token_end_of_stream,
	token_error,
	token_number,
	token_vertex_comp,
	token_vertex_texture_comp,
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
	u32 start;
	u32 size;
    struct importer_element_t *next_sibling;
	u32 index;
	enum importer_token_type type;
	
} importer_element_t;

typedef struct
{
	buffer_t source;
	u64 at;
	b32 had_error;
	
} mesh_importer_t;

internal_f b32 
is_white_space(buffer_t _source, u64 _at)
{
	b32 result = false;	
	if(is_in_bounds(_source, _at))
	{
		u8 val = BUFFER_DATA(_source, u8, _at);
		result = ((val == ' ') || (val == '\t') || (val == '\n') || (val == '\r'));
	}
	
	return result;
}

internal_f b32
is_digit(buffer_t _source, u64 _at)
{
	b32 result = false;
	if(is_in_bounds(_source, _at))
	{
		u8 val = BUFFER_DATA(_source, u8, _at);
		result = ((val >= '0') && (val <= '9'));
	}
	
	return result;
}




////////////////////
//In this case for the importer I have decided to add a node for each elemenet of the imported data.
// e. g: vertex_coord(x, y, z) -> x, y and z are different elements.
//
// So when retrieving the num of each component like the verteces, we would have to divided by 2.
///////////////////
internal_f importer_element_t*
import_mesh(arena_t *_temp_arena, const mesh_importer_t *_importer)
{	
	buffer_t source = _importer->source;
	u64 at = _importer->at;
	importer_element_t *first_element = 0;
	importer_element_t *element_ptr = 0;
	
	u32 counter = 0;
	
	while(is_in_bounds(source, at))
	{		
		
		// Skiping white spaces
		while(is_white_space(source, at))
		{
			++at;
		}
		
		u8 val = ((u8*)source.data)[at++];
		
		////////////
		// IMPORTANT - At is currently in the next byte and Val is the prev, we check for val.ks
		////////////
		
		// we move the parsing pointer until we reach something significant and save the buffer.
		switch(val)
		{					
			case 'v':
			{
				// potential vertex and other data			
				u64 start = at;
				if((is_in_bounds(source, at)))
				{	
					u8 next = BUFFER_DATA(source, u8, at);
					if(next == ' ')
					{
						// advancing to the number
						start = ++at;
						// parse until next vertex components
						while(is_in_bounds(source, at))
						{
							u8 val = BUFFER_DATA(source, u8, at);
							bool bOut = val == '\n';
							// end of a number
							if(val == ' ' || val == '\n' || val == '\r' || val == '\t')
							{								
								importer_element_t *vertex_element = push_struct(_temp_arena, importer_element_t);
								vertex_element->type = token_vertex_comp;
								vertex_element->start = start;
								vertex_element->size = at - start;	
								
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
								
								if(bOut)
								{
									break;
								}
								
							}
							else
							{
								at++;							
							}
						}
					}
					else if(next == 't')
					{
						// Vertex texture coordinate
						
						// advance to the current number
						at += 2;
						start = at;
						u8 val = BUFFER_DATA(source, u8, at);
						while(is_in_bounds(source, at))
						{
							u8 val = BUFFER_DATA(source, u8, at);
							bool bOut = val == '\n';
							
							// end of a number
							
							if(val == ' ' || val == '\n' || val == '\r' || val == '\t')
							{
								importer_element_t *vt_element = push_struct(_temp_arena, importer_element_t);
								vt_element->type = token_vertex_texture_comp;
								vt_element->start = start;
								vt_element->size = at - start;	
								
								if(!first_element)
								{
									first_element = vt_element;
								}
								
								if(element_ptr)
								{
									element_ptr->next_sibling = vt_element;
								}
								
								element_ptr = vt_element;
								start = ++at;
								if(bOut)
								{
									break;
								}
							}
							else
							{
								at++;							
							}
						}
						
					}
					
				}
				
			}break;
			
			case 'f':
			{			
				u64 start = at;
				
				// parse until the next face which is f in the file.
				if(is_in_bounds(source, at) && (BUFFER_DATA(source, u8, at) == ' '))
				{				
					start = ++at;
					
					int face_comp_count = 0;
					
					while (is_in_bounds(source, at))
					{
						u8 val = BUFFER_DATA(source, u8, at);
						
						bool bOut = val == '\n';
						
						if ((val == ' ') || (val == '\n') || (val == '\r') || (val == '\t'))
						{
							if (at > start) 
							{
								importer_element_t *face_element = push_struct(_temp_arena, importer_element_t);
								face_element->next_sibling = 0;
								face_element->index = counter++;
								face_element->type = token_face_comp;
								face_element->start = start;
								face_element->size = at - start;
								
								if (!first_element) first_element = face_element;
								if (element_ptr) element_ptr->next_sibling = face_element;
								element_ptr = face_element;
								
								face_comp_count++;
																
							}
							
							start = ++at;
							
							if(bOut)
							{
								break;
							}
						}
						else
						{
							at++;
						}
					}
					
					if (face_comp_count != 3)
					{
						printf("⚠️ Face had %d components instead of 3!\n", face_comp_count);
					}
					
					
					
				}break;
							
			}
			
			default:
			{
				// Skip the rest of the line for any unhandled prefix (like 'vn', 's', etc.)
				while (is_in_bounds(source, at) && BUFFER_DATA(source, u8, at) != '\n')
				{
					++at;
				}
				++at; // Skip the newline character
				
			} break;
		}
	}
	
	printf("COUNTER: %i \n", counter);
	
	
	return first_element;
}


enum enum_parsing
{
	category_none = 0,
	category_vertex = 1,
	category_face = 2,
};


internal_f f64 
convert_to_sign(buffer_t _source, u64 *_at_result)
{
	u64 at = *_at_result;
	
	f64 result = 1.0;
	if(is_in_bounds(_source, at) && (BUFFER_DATA(_source, u8, at) == '-'))
	{
		result = -1.0;
		++at;
	}
	
	*_at_result = at;		
	return result;
}

internal_f f64 
convert_to_number(buffer_t _source, u64 *_at_result)
{
	u64 at = *_at_result;
	
	f64 result = 0.0f;
	while(is_in_bounds(_source, at))	
	{
		u8 value = BUFFER_DATA(_source, u8, at) - (u8)'0';
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

internal_f f64
convert_to_f64(buffer_t _source)
{
	f64 result = 0.0;
	
	buffer_t source = _source;
	u64 at = 0;
	
	f64 sign = convert_to_sign(source, &at);
	f64 number = convert_to_number(source, &at);
	
	if(is_in_bounds(source, at) && BUFFER_DATA(source, u8, at) == '.')
	{
		++at;
		f64 C = 1.0 / 10.0;
		while(is_in_bounds(source, at))
		{
			u8 value = BUFFER_DATA(source, u8, at) - (u8)'0';
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

internal_f vec3_t 
convert_to_vertex(importer_element_t *first_vertex_component, buffer_t buffer)
{
	vec3_t result;
	result.x = 0;
	result.y = 0;
	result.z = 0;
		
	buffer_t x_buff;
	x_buff.data = BUFFER(buffer, u8) + first_vertex_component->start;
	x_buff.size = first_vertex_component->size;
	
	buffer_t y_buff;
	y_buff.data = BUFFER(buffer, u8) + first_vertex_component->next_sibling->start;
	y_buff.size = first_vertex_component->next_sibling->size;
	
	buffer_t z_buff;
	z_buff.data = BUFFER(buffer, u8) + first_vertex_component->next_sibling->next_sibling->start;
	z_buff.size = first_vertex_component->next_sibling->next_sibling->size;
	
	f64 x = convert_to_f64(x_buff);
	f64 y = convert_to_f64(y_buff);
	f64 z = convert_to_f64(z_buff);
	
	
	result.x = (f32)x;
	result.y = (f32)y;
	result.z = (f32)z;
	
	return result;
}

internal_f void 
convert_to_texture_coordinate(buffer_t buffer, importer_element_t *element, f32 *u, f32 *v)
{
	*u = 0;
	*v = 0;
	
	buffer_t u_buff;
	u_buff.data = BUFFER(buffer, u8) + element->start;
	u_buff.size = element->size;
		
	buffer_t v_buff;
	v_buff.data = BUFFER(buffer, u8) + element->next_sibling->start;
	v_buff.size = element->next_sibling->size;	
	
	*u = convert_to_f64(u_buff);
	*v = convert_to_f64(v_buff);	
}

internal_f face_t
convert_to_face(mesh_t *_mesh, importer_element_t *first_face_element, buffer_t source)
{
	// each face element has 3 components a, b and c, which the a is the only element that we care about for now
	// v1/vt1/vn1
	face_t result;
	
	// TODO:(optimization) use the same buffer here, in this case we can use a single node for all the face and do the parsing here, better.
	// Let's make this an array moduled
	u64 at = 0;
	buffer_t a_buffer;
	a_buffer.data = BUFFER(source, u8) + first_face_element->start;
	a_buffer.size = first_face_element->size;
	result.a = (s32)convert_to_number(a_buffer, &at) - 1;
		
	
	at++;
	u32 uv_index = (u32)convert_to_number(a_buffer, &at) - 1;
	texture_uv_t temp_uv = _mesh->uv_coords[uv_index];
	result.a_uv = temp_uv;

	
	at = 0;
	buffer_t b_buffer;
	b_buffer.data = BUFFER(source, u8) + first_face_element->next_sibling->start;
	b_buffer.size = first_face_element->next_sibling->size;
	result.b = (s32)convert_to_number(b_buffer, &at) - 1;
	
	
	at++;;
	uv_index = (u32)convert_to_number(b_buffer, &at) - 1;
	temp_uv = _mesh->uv_coords[uv_index];
	result.b_uv = temp_uv;
	
	
	at = 0;
	buffer_t c_buffer;
	c_buffer.data = BUFFER(source, u8) + first_face_element->next_sibling->next_sibling->start;
	c_buffer.size = first_face_element->next_sibling->next_sibling->size;
	result.c = (s32)convert_to_number(c_buffer, &at) - 1;
		
	at++;
	uv_index = (u32)convert_to_number(c_buffer, &at) - 1;
	temp_uv = _mesh->uv_coords[uv_index];
	result.c_uv = temp_uv;
	

		//printf("face: a: %i, b: %i, c: %i \n", result.a, result.b, result.c);
		
		return result;
	}
	
	
	// For now let's make this simple, so we have an array of structs,
	// I would like to change this to struct of arrays, so we can have id DOD and better for cache locality
	// in this case, maybe allocting everything in the temp memory and then passing it to the permanent memory?..
	internal_f mesh_t
	create_mesh_from_file(memory_t *engine_memory, const char *_file_name)
	{
		mesh_t result;
		result.face_num = 0;
		result.vertex_num = 0;
		result.uv_coords_num = 0;
		result.rotation.x = 0;
		result.rotation.y = 0;
		result.rotation.z = 0;
		result.scale.x = 1;
		result.scale.y = 1;
		result.scale.z = 1;
		
		if (_file_name == 0)
		{
			return result;
		}	
			
		SCRATCH();
		
		buffer_t buffer = read_file(temp_arena, _file_name);
		if (buffer.size > 0)
		{						
			printf("Importing mesh %s \n", _file_name);
			mesh_importer_t importer;
			importer.source = buffer;
			importer.at = 0;
			importer.had_error = false;		
			
			importer_element_t *element = import_mesh(temp_arena, &importer);
			
			s32 count_face = 0;
			s32 count_vertex = 0;
			s32 iterated = 0;
			
			
			// convert to while and advace 3;
			// note:(juanes.rayo): add the same thing for the faces.
			// TODO: Optimization here, don't iterate through the faces twice, add the num while creating the components of the list.
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
				else if(it->type == token_vertex_texture_comp)
				{
					result.uv_coords_num++;
				}
			}
			
			result.vertex_num /= 3;
			result.face_num /= 3;
			result.uv_coords_num /= 2;
			printf("iterated ver: %i \n", result.vertex_num);
			printf("iterated face: %i \n", result.face_num);		
			printf("iterated uv_corods: %i \n", result.uv_coords_num);		
			
			result.verteces = push_array(&engine_memory->permanent, result.vertex_num, vec3_t);
			result.faces = push_array(&engine_memory->permanent, result.face_num, face_t);
			result.uv_coords = push_array(&engine_memory->permanent, result.uv_coords_num, texture_uv_t);
			
			//(juanes.rayo) NOTE: can we figure this out runtime? or is better to store it too? as we have already the verteces and the faces, we could figure this out runtime.
			//result.triangles = PushArray(&engine_memory->permanent, count_face, triangle_t);
			
			u32 current_vertex = 0;
			u32 current_face = 0;				
			u32 current_uv = 0;
			
			f64 temp_vertex_sign = 1.0f;
			
			// Face and Vertex fill
					
			importer_element_t *it = element;		
			while(it != 0)
			{			
				if(it->type == token_face_comp)
				{
					face_t face = convert_to_face(&result, it, importer.source);
					result.faces[current_face++] = face;
					it = it->next_sibling->next_sibling->next_sibling;
				}
				else if(it->type == token_vertex_comp)
				{								
					vec3_t vertex = convert_to_vertex(it, importer.source);				
					
					result.verteces[current_vertex] = vertex;
					current_vertex++;
					
					it = it->next_sibling->next_sibling->next_sibling;
				}
				else if (it->type == token_vertex_texture_comp)
				{
					f32 u = 0;
					f32 v = 0;
					convert_to_texture_coordinate(importer.source, it, &u, &v);
					
					texture_uv_t uv;
					uv.u = u;
					uv.v = v;				
					result.uv_coords[current_uv++] = uv;
					it = it->next_sibling->next_sibling;
					
				}
			}
			
			// Checking quantity equality between the parsed faces and vertex num and the ones pushed to the mesh_t.
			
			//printf("parsed: %i result %i \n", current_face, result.face_num );
			
			assert(current_face == result.face_num);
			assert(current_vertex == result.vertex_num);	
			assert(current_uv == result.uv_coords_num);
		}
		
		
		
		SCRATCH_END();
				
	
		return result;
	}
	