/* date = October 31st 2025 0:36 pm */

// For now lets just return the key_size
u32 texture_buffer_hash(void* key, u32 key_size)
{
	return key_size;
}

global_f void
texture_manager_init(arena_t *_arena, texture_manager_t *_manager)
{	
	_manager->texture_map = hash_map_create(_arena, 20, 0, sizeof(texture_t), &hash_function_c_string);
}


global_f void
texture_manager_add_texture(texture_manager_t *_manager, string_t _texture_name, texture_t _texture)
{	
	HASH_MAP_STR_ADD(_manager->texture_map, _texture_name.buffer.data, &_texture);
}


global_f texture_t* 
texture_manager_get_texture(texture_manager_t *_manager, string_t texture_name)
{
	if(texture_name.size > 0)
	{				
		texture_t *found_data = 0;
		HASH_MAP_STR_FIND(_manager->texture_map, texture_t, texture_name.buffer.data, found_data);
		return found_data;
	}
	
	return 0;
}


