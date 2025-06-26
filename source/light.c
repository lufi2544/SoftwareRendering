
/////////////////////////////////
////////// LIGHT RELATED LOGIC



/**
* Applies intensity to a color.
* NOTE: Pixel Format = ARGB8888 -> 32 bit packed bits.
*/
global_f color_t
light_apply_intensity(color_t _original_color, f32 _percentage)
{
	if (_percentage < 0)
	{
		_percentage = 0;
	}
	
	if (_percentage > 1)
	{
		_percentage = 1;
	}
	
	u32 a = _original_color & 0xFF000000;
	u32 r = ((_original_color >> 16) & 0xFF) * _percentage;
	u32 g = ((_original_color >> 8)  & 0xFF) * _percentage;
	u32 b = ((_original_color >> 0)  & 0xFF) * _percentage;	
	
	color_t new_color = a | (r << 16) | (g << 8) | (b);	
	return new_color;
}



// Having in mind that 255 is the max, we would have to figure out a method in which dot == -1 means a lot of light and 
// dot == 1, means not too much light
global_f color_t
light_flat_pass(light_t _light_point, vec3_t _normal, color_t _triangle_color)
{
	// Gradiant for more lit or less
	f32 dot = -vec3_dot(_normal, _light_point.direction);		
	return light_apply_intensity(_triangle_color, dot);
}