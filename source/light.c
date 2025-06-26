
/////////////////////////////////
////////// LIGHT RELATED LOGIC



/**
* Applies intensity to a color.
* NOTE: Pixel Format = ARGB8888 -> 32 bit packed bits.
*/
global_f
 color_t light_apply_intensity(color_t _original_color, f32 _percentage)
{
	_percentage /= 100;
	u8 a = _original_color & 0xFF000000;
	u8 r = (_original_color & 0x00FF0000) * _percentage;
	u8 g = (_original_color & 0x0000FF00) * _percentage;
	u8 b = (_original_color & 0x000000FF) * _percentage;
	
	color_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);	
	return new_color;
}



// Having in mind that 255 is the max, we would have to figure out a method in which dot == -1 means a lot of light and 
// dot == 1, means not too much light
global_f
color_t lighti_flat_pass(light_t _light_point, vec3_t _normal, color_t _triangle_color)
{
	// Gradiant for more lit or less
	f32 gradiant = 10;
	f32 dot = vec3_dot(_normal, _light_point.direction);
	
	if (dot > 0)
	{
		
	}
	else
	{
		
	}
	
	
	return 0;
}