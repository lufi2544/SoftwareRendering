
internal void
draw_pixel(vec2_t position , u32 color)
{
	if(position.x < 0 || position.y < 0 || position.x >= window_width || position.y >= window_height)
	{
		return;
	}
	
	u32 Index = (window_width * ((u32)position.y)) + ((u32)(position.x));
	color_buffer[Index] = color;
}

// Draws a rectangle on the screen at a certain coordinate X and Y with a Color.
internal void
draw_rect(s32 x, s32 y, s32 w, s32 h, u32 color)
{
    for(s32 height_index = y; height_index < y + h; ++height_index)
    {
        for(s32 width_index = x; width_index < x + w; ++width_index)
        {
            if(width_index < window_width && height_index < window_height)
            {                
                vec2_t pixel_position = { width_index, height_index };
                draw_pixel(pixel_position, color);
            }
        }
    }
    
}

internal void
draw_line(s32 x0, s32 y0, s32 x1, s32 y1)
{
	// using the DDA algorithm for now, which has in count the rise of the line to draw the pixels
	// for the line
	
	const s32 delta_x = (x1 - x0);
	const s32 delta_y = (y1 - y0);
	const s32 delta_x_abs = abs(delta_x);
	const s32 delta_y_abs = abs(delta_y);
	
	// If the x increment is greater than the y increment, then the x is incrementing by 1 and the Y by a smaller amount,
	// if the y is greater, then is the other way around, so we have to have that in mind to calculate the side lenght
	const s32 side_lenght = delta_x_abs >= delta_y_abs ? delta_x_abs : delta_y_abs;
	
	f32 x_inc = delta_x / (f32)side_lenght;
	f32 y_inc = delta_y / (f32)side_lenght;
	
	f32 current_x = x0;
	f32 current_y = y0;
	
	for(s32 i = 0; i <= side_lenght; ++i)
	{
		vec2_t position = { round(current_x), round(current_y) };
		draw_pixel(position, 0x00000000);
		current_x += x_inc;
		current_y += y_inc;
	}	
}

internal void 
draw_linear_triangle(triangle_t *_triangle)
{
	vec2_t position_0 = {_triangle->points[0].x, _triangle->points[0].y};
	draw_rect(position_0.x, position_0.y, 5, 5, 0x000000);			
	
	vec2_t position_1 ={_triangle->points[1].x, _triangle->points[1].y};
	draw_rect(position_1.x, position_1.y, 5, 5, 0x000000);				
	
	vec2_t position_2 ={_triangle->points[2].x, _triangle->points[2].y};
	draw_rect(position_2.x, position_2.y, 5, 5, 0x000000);
	
	// A-B
	draw_line(position_0.x, position_0.y, position_1.x, position_1.y);
	
	// B-C
	draw_line(position_1.x, position_1.y, position_2.x, position_2.y);
	
	// C-A
	draw_line(position_2.x, position_2.y, position_0.x, position_0.y);
}

internal void 
f32_swap_values(f32 *a, f32 *b)
{
	f32 temp = *a;
	*a = *b;
	*b = temp;
	
}

internal void
draw_filled_triangle(triangle_t *_triangle)
{
	// sort the verteces of the triangle
	vec2_t a = _triangle->points[0];
	vec2_t b = _triangle->points[1];
	vec2_t c = _triangle->points[2];
	
	// sort y 
	if (a.y > b.y)
	{
		f32_swap_values(&a.y, &b.y);
		f32_swap_values(&a.x, &b.x);
	}	
	
	if (b.y > c.y)	
	{
		f32_swap_values(&b.y, &c.y);
		f32_swap_values(&b.x, &c.x);
	}
	
	if (a.y > b.y)	
	{
		f32_swap_values(&a.y, &b.y);
		f32_swap_values(&a.x, &b.x);
	}
	
	// Calculate the new vertex Mx and My
	f32 mx = a.x + (c.x - a.x) * ((b.y - a.y) / (c.y - a.y));
	
	f32 my = b.y;
	
	vec2_t m = { mx, my };
	
	triangle_t top_triangle;
	top_triangle.points[0] = a;
	top_triangle.points[1] = m;
	top_triangle.points[2] = b;	
	
	triangle_t bottom_triangle;
	bottom_triangle.points[0] = b;
	bottom_triangle.points[1] = m;
	bottom_triangle.points[2] = c;
	
	draw_linear_triangle(&top_triangle);
	draw_linear_triangle(&bottom_triangle);	
}