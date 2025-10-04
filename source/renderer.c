

////////////// RENDERER //////////////

#define GRID_DEFAULT_COLOR 0xA0A0A0

// TODO figure out where to put the projection matrix.
internal_f void
render(mat4_t *projection_matrix, memory_t *engine_memory, engine_shared_data_t *data)
{
    
    SDL_RenderClear(renderer);   
    map_texture_to_pixels_buffer(data);    
	
	        
	clear_color_buffer(COLOR_BLACK, data);
	g_app_code.app_render(engine_memory, data);
	
	for(u32 i = 0; i < data->meshes_num; ++i)
	{
		printf("Mesh %i: faces=%i, verts=%i\n", i, data->meshes[i].face_num, data->meshes[i].vertex_num);
		mesh_render(engine_memory, &data->meshes[i], &data->camera, projection_matrix, data);
	}
    
    SDL_RenderPresent(renderer);
}


bool
render_settings_check_flag(render_settings *_render_settings, enum_render_settings_flags flag)
{
	u32 mask = 1 << (u32)flag;
	
	return _render_settings->flags & mask;
}

internal_f void
render_settings_set_flag(render_settings *_render_settings, enum_render_settings_flags flag, bool bValue)
{
	u32 mask = (1 << (u32)flag);
	if(bValue)
	{
		_render_settings->flags |= mask;
	}
	else
	{
		_render_settings->flags &= (~mask);
	}
}

internal_f void
draw_pixel(vec2_t position , u32 color, engine_shared_data_t *engine_data)
{
	if(position.x < 0 || position.y < 0 || position.x >= engine_data->window_width || position.y >= engine_data->window_height)
	{
		return;
	}
	
	u32 Index = (engine_data->window_width* ((u32)position.y)) + ((u32)(position.x));
	color_buffer[Index] = color;
}

// Draws a rectangle on the screen at a certain coordinate X and Y with a Color.
internal_f void
draw_rect(s32 x, s32 y, s32 w, s32 h, u32 color, engine_shared_data_t *engine_data)
{
    for(s32 height_index = y; height_index < y + h; ++height_index)
    {
        for(s32 width_index = x; width_index < x + w; ++width_index)
        {
            if(width_index < engine_data->window_width && height_index < engine_data->window_height)
            {                
                vec2_t pixel_position = { width_index, height_index };
                draw_pixel(pixel_position, color, engine_data);
            }
        }
    }
    
}

internal_f void
draw_line(s32 x0, s32 y0, s32 x1, s32 y1, u32 _color, engine_shared_data_t *engine_data)
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
		draw_pixel(position, _color, engine_data);
		current_x += x_inc;
		current_y += y_inc;
	}	
}

internal_f void 
draw_linear_triangle(triangle_t *_triangle, u32 _color, bool bDrawDots, engine_shared_data_t *engine_data)
{
	vec2_t position_0 = {_triangle->points[0].x, _triangle->points[0].y};
	vec2_t position_1 ={_triangle->points[1].x, _triangle->points[1].y};
	vec2_t position_2 ={_triangle->points[2].x, _triangle->points[2].y};
	
	if(bDrawDots)
	{				
		draw_rect(position_0.x, position_0.y, 5, 5, _color, engine_data);
		draw_rect(position_1.x, position_1.y, 5, 5, _color, engine_data);
		draw_rect(position_2.x, position_2.y, 5, 5, _color, engine_data);
	}
	
	// A-B
	draw_line(position_0.x, position_0.y, position_1.x, position_1.y, _color, engine_data);
	
	// B-C
	draw_line(position_1.x, position_1.y, position_2.x, position_2.y, _color, engine_data);
	
	// C-A
	draw_line(position_2.x, position_2.y, position_0.x, position_0.y, _color, engine_data);
}

internal_f void 
f32_swap_values(f32 *a, f32 *b)
{
	f32 temp = *a;
	*a = *b;
	*b = temp;
	
}

internal_f void 
fill_flat_triangle_top(triangle_t *_triangle, u32 _color, engine_shared_data_t *engine_data)
{
	// NOTE:(ishak) We have to  calculate the inv slope here so Delta X / Delta Y.
	vec2_t a = _triangle->points[0];
	vec2_t b = _triangle->points[1];
	vec2_t c = _triangle->points[2];		
	
	s32 x0 = a.x;
	s32 y0 = a.y;
	
	s32 x1 = b.x;
	s32 y1 = b.y;
	
	s32 x2 = c.x;
	s32 y2 = c.y;		
	
	//  printf("x0 %i y0 %i x1 %i y1 %i x2 %i y2 %i \n", x0, y0, x1, y1, x2, y2);
	
	f32 inv_slope_1 = ((f32)(x1 - x0) / (y1 - y0));
	f32 inv_slope_2 = ((f32)(x2 - x0) / (y2 - y0));
	
	// from a to b incrementing y by the inv slope	
	f32 x_start = x0;
	f32 x_end = x0;
	for (s32 y = y0; y <= y2; ++y)
	{
	    //printf("s:%.4f, e: %.4f \n", x_start, x_end);
		draw_line(x_start, y, x_end, y, _color, engine_data);
		x_start += inv_slope_1;
		x_end += inv_slope_2;
	}
	
	//	printf("--------- \n");
	
}

internal_f void 
fill_flat_triangle_bottom(triangle_t *_triangle, u32 _color, engine_shared_data_t *engine_data)
{
	// NOTE:(ishak) We have to  calculate the inv slope here so Delta X / Delta Y.
	vec2_t a = _triangle->points[0];
	vec2_t b = _triangle->points[1];
	vec2_t c = _triangle->points[2];
	
	//the bottom is b here.
	
	s32 x0 = b.x;
	s32 y0 = b.y;
	
	s32 x1 = a.x;
	s32 y1 = a.y;
	
	s32 x2 = c.x;
	s32 y2 = c.y;
	
	
	f32 inv_slope_1 = ((f32)(x1 - x0) / (f32)(y1 - y0));
	f32 inv_slope_2 = ((f32)(x2 - x0) / (f32)(y2 - y0));
	
	//printf("p1: %.4f, p2: %.4f \n", inv_slope_1, inv_slope_2);
	
	// from a to b incrementing y by the inv slope	
	f32 x_start = x0;
	f32 x_end = x0;
	for (s32 y = y0; y > y2; y--)
	{
		draw_line(x_start, y, x_end, y, _color, engine_data);
		x_start -= inv_slope_1;
		x_end -= inv_slope_2;
	}
	
}


internal_f void
draw_textured_triangle(triangle_t *_triangle, texture_t _texture)
{
	
}

void
draw_filled_triangle(triangle_t *_triangle, u32 _color, engine_shared_data_t *engine_data)
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
	
	triangle_t top_triangle;
	triangle_t bottom_triangle;
	bottom_triangle.points[0] = a;
	bottom_triangle.points[1] = b;
	bottom_triangle.points[2] = c;
	
	
	if(b.y == c.y)
	{		
		fill_flat_triangle_top(&bottom_triangle, _color, engine_data);
		
	} 
	else if(a.y == b.y)
	{
		fill_flat_triangle_bottom(&bottom_triangle, _color, engine_data);
	}
	else
	{
		
		// Calculate the new vertex Mx and My
		f32 mx = a.x + ((f32)(c.x - a.x) * ((b.y - a.y)) / (f32)(c.y - a.y));
		
		f32 my = b.y;
		
		vec2_t m = { mx, my };
		
		top_triangle.points[0] = a;
		top_triangle.points[1] = b;
		top_triangle.points[2] = m;	
		
		bottom_triangle.points[0] = b;
		bottom_triangle.points[1] = c;
		bottom_triangle.points[2] = m;
		
		
		fill_flat_triangle_bottom(&bottom_triangle, _color, engine_data);
		fill_flat_triangle_top(&top_triangle, _color, engine_data);
	}
	
}
