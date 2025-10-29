/* date = June 27th 2025 9:51 am */

typedef u32 color_t;

// We use the  UV coordinate method for mapping the texture to the faces.
typedef struct
{
	color_t *buffer;
	s32 w;
	s32 h;	
} texture_t;



internal_f color_t
get_color_from_texture(texture_t *_texture, s32 _x, s32 _y)
{
	color_t result;
	
	result = _texture->buffer[(_texture->w * _y) + _x];
	return result;
}
