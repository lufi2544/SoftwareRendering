/* date = June 27th 2025 9:51 am */

internal_f color_t
get_color_from_texture(texture_t *_texture, s32 _x, s32 _y)
{
	color_t result;
	
	result = _texture->buffer[(_texture->w * _y) + _x];
	return result;
}
