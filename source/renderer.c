


////////////// RENDERER //////////////

#define GRID_DEFAULT_COLOR 0xA0A0A0

global_f void 
renderer_init(engine_shared_data_t *shared_data)
{
	texture_manager_init(&g_memory.permanent, &shared_data->texture_manager);
	
}

// TODO figure out where to put the projection matrix.
internal_f void
render(mat4_t *projection_matrix, engine_shared_data_t *engine_data)
{
    
    SDL_RenderClear(renderer);   
    map_texture_to_pixels_buffer(engine_data);    	
    
	clear_color_buffer(COLOR_BLACK, engine_data);
	clear_z_buffer(engine_data);
	g_app_code.app_render(engine_data);
	
	for(u32 i = 0; i < engine_data->meshes_num; ++i)
	{
		mesh_render(engine_data->memory, &engine_data->meshes[i], &engine_data->camera, projection_matrix, engine_data);
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
int_swap(s32 *a, s32 *b)
{
	s32 temp = *a;
	*a = *b;
	*b = temp;
}

internal_f vec3_t 
barycentric_weights(vec2_t _a, vec2_t _b, vec2_t _c, vec2_t _p)
{
	vec2_t pc = vec2_subtract(_c, _p);
	vec2_t pb = vec2_subtract(_b, _p);
	
	vec2_t ac = vec2_subtract(_c, _a);
	vec2_t ab = vec2_subtract(_b, _a);
	
	vec2_t ap = vec2_subtract(_p, _a);
	
	f32 alpha = vec2_cross(pc, pb) / vec2_cross(ac, ab);
	f32 beta = vec2_cross(ac, ap) / vec2_cross(ac, ab);
	f32 gamma = 1 - (beta) - (alpha); 
	
	vec3_t result = { alpha, beta, gamma };	
	return result;
}

internal_f void
draw_pixel_color(color_t _color, s32 _x, s32 _y, vec4_t _a, vec4_t _b, vec4_t _c, engine_shared_data_t *engine_shared_data)
{
	
	if(_x <0 || _x >= engine_shared_data->window_width || _y < 0 ||_y >= engine_shared_data->window_height)
	{
		return;
	}
	
	vec2_t p = {_x, _y};
	vec3_t weights = barycentric_weights(vec2_from_vec4(_a), vec2_from_vec4(_b), vec2_from_vec4(_c), p);
	
	f32 alpha = weights.x;
	f32 beta = weights.y;
	f32 gamma = weights.z;
	
	f32 interpolated_reciprocal_w;
	interpolated_reciprocal_w = ((1 / _a.w) * alpha) + ((1 / _b.w) * beta) + ((1 / _c.w) * gamma);
	
	if(interpolated_reciprocal_w < z_buffer[(engine_shared_data->window_width * _y) + _x])
	{
		vec2_t position ={ _x, _y};
		draw_pixel(position, _color, engine_shared_data);
		z_buffer[(engine_shared_data->window_width * _y) + _x] = interpolated_reciprocal_w;
	}
}


internal_f void
draw_texel(s32 _x, s32 _y, vec4_t _a, vec4_t _b, vec4_t _c, texture_uv_t _uv_a, texture_uv_t _uv_b, texture_uv_t _uv_c, texture_t *_texture, engine_shared_data_t *engine_shared_data)
{
	
	if(_x < 0 || _x >= engine_shared_data->window_width || _y < 0 ||_y >= engine_shared_data->window_height)
	{
		return;
	}
	
	vec2_t p = {_x, _y};
	vec3_t weights = barycentric_weights(vec2_from_vec4(_a), vec2_from_vec4(_b), vec2_from_vec4(_c), p);
	
	f32 alpha = weights.x;
	f32 beta = weights.y;
	f32 gamma = weights.z;
	
	f32 interpolated_u;
	f32 interpolated_v;
	f32 interpolated_reciprocal_w;
	
	interpolated_u = ((_uv_a.u / _a.w) * alpha) + ((_uv_b.u / _b.w) * beta) + ((_uv_c.u / _c.w) * gamma);
	interpolated_v = ((_uv_a.v / _a.w) * alpha) + ((_uv_b.v / _b.w) * beta) + ((_uv_c.v / _c.w) * gamma);
	interpolated_reciprocal_w = ((1 / _a.w) * alpha) + ((1 / _b.w) * beta) + ((1 / _c.w) * gamma);
	
	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;
    
	// Convert normalized UV coordinates (0.0-1.0) to texture pixel coordinates
	s32 tex_x = (s32)(interpolated_u * _texture->w);
	s32 tex_y = (s32)(interpolated_v * _texture->h);
    
	// Clamp texture coordinates to valid range
	if(tex_x < 0) tex_x = 0;
	if(tex_x >= _texture->w) tex_x = _texture->w - 1;
	if(tex_y < 0) tex_y = 0;
	if(tex_y >= _texture->h) tex_y = _texture->h - 1;
	
	if(interpolated_reciprocal_w < z_buffer[(engine_shared_data->window_width * _y) + _x])
	{
		u32 texture_color = get_color_from_texture(_texture, tex_x, tex_y);	
		
		vec2_t position ={ _x, _y};
		draw_pixel(position, texture_color, engine_shared_data);
		z_buffer[(engine_shared_data->window_width * _y) + _x] = interpolated_reciprocal_w;
	}
}


#define MAX_TEXTURED_TRIANGLE_SLOPE 50


// ---- TEXTURED RENDERING ----
internal_f f32 safe_inv_slope(float dx, float dy)
{
    const float EPS = 1.0f; // Minimum meaningful vertical delta
	
    if (fabsf(dy) < EPS) {
        // Edge is almost horizontal → extremely thin triangle in Y
        // Return 0 so we don't explode the slope
        return 0.0f;
    }
	
	
    return dx / (dy);
}

/*
 * We render the top triangle here, from a.y to b.y, with having in mind that the points are sorted by y.
*/
internal_f void
fill_triangle_top_textured(triangle_t *_triangle, texture_t *_texture, engine_shared_data_t *engine_shared_data)
{
    
	// Render the upper part of the triangle (flat-bottom): from y0 to y1
	vec4_t a = _triangle->points[0];  // y0 (top)
	vec4_t b = _triangle->points[1];  // y1 (middle)
	vec4_t c = _triangle->points[2];  // y2 (bottom)
	
	texture_uv_t a_uv = _triangle->texture_coords[0];
	texture_uv_t b_uv = _triangle->texture_coords[1];
	texture_uv_t c_uv = _triangle->texture_coords[2];
    
	f32 inv_slope_1 = 0;
	f32 inv_slope_2 = 0;
    
	// Calculate inverse slopes (dx/dy) for left and right edges
	// Only check for division by zero
	if (b.y - a.y != 0)
	{
		inv_slope_1 = safe_inv_slope((b.x - a.x) , ((b.y - a.y)));
        
		f32_clamp(&inv_slope_1, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
    
	if (c.y - a.y != 0)
	{
		inv_slope_2 = safe_inv_slope((c.x - a.x), (c.y - a.y));
		f32_clamp(&inv_slope_2, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
	
	
	if (b.y - a.y != 0)
	{
		for(s32 y = a.y; y <= b.y; y++)
		{
			s32 x_start = a.x + (y - a.y) * inv_slope_1;
			s32 x_end = a.x + (y - a.y) * inv_slope_2;
			
			
			if(x_end < x_start)
			{
				int_swap(&x_start, &x_end);
			}
            
			for(s32 x = x_start; x < x_end; x++)
			{
				draw_texel(x, y, a, b, c, a_uv, b_uv, c_uv, _texture, engine_shared_data);
			}
		}
	}
}


/*
We render the bottom triangle, so from c.y to the b.y. 
*/
internal_f void
fill_triangle_bottom_textured(triangle_t *_triangle, texture_t *_texture, engine_shared_data_t *engine_shared_data)
{
	vec4_t a = _triangle->points[0];  // y0 (top)
	vec4_t b = _triangle->points[1];  // y1 (middle)
	vec4_t c = _triangle->points[2];  // y2 (bottom)
    
	texture_uv_t a_uv = _triangle->texture_coords[0];
	texture_uv_t b_uv = _triangle->texture_coords[1];
	texture_uv_t c_uv = _triangle->texture_coords[2];
    
	f32 inv_slope_1 = 0;
	f32 inv_slope_2 = 0;
    
	// Calculate inverse slopes (dx/dy) for left and right edges
	// Only check for division by zero
	if (c.y - b.y != 0)
	{
		inv_slope_1 = safe_inv_slope((b.x - c.x), (b.y - c.y));
		f32_clamp(&inv_slope_1, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
    
	if (c.y - a.y != 0)
	{
		inv_slope_2 = safe_inv_slope((a.x - c.x), (a.y - c.y));
		f32_clamp(&inv_slope_2, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
	
    
	if (c.y - b.y != 0)
	{
		for(s32 y = c.y; y >= b.y; --y)
		{
			s32 x_start = c.x + (y - c.y) * inv_slope_1;
			s32 x_end = c.x + (y - c.y) * inv_slope_2;
            
			if(x_end < x_start)
			{
				int_swap(&x_start, &x_end);
			}
            
			for(s32 x = x_start; x < x_end; x++)
			{
				draw_texel(x, y, a, b, c, a_uv, b_uv, c_uv, _texture, engine_shared_data);
			}
		}
	}
}

// ---- FLAT RENDERING ----

internal_f void 
fill_flat_triangle_bottom(triangle_t *_triangle, u32 _color, engine_shared_data_t *engine_data)
{
	vec4_t a = _triangle->points[0];  // y0 (top)
	vec4_t b = _triangle->points[1];  // y1 (middle)
	vec4_t c = _triangle->points[2];  // y2 (bottom)		
	
	f32 inv_slope_1 = 0;
	f32 inv_slope_2 = 0;
	
	// Calculate inverse slopes (dx/dy) for left and right edges
	// Only check for division by zero
	if (c.y - b.y != 0)
	{
		inv_slope_1 = safe_inv_slope((b.x - c.x), (b.y - c.y));
		f32_clamp(&inv_slope_1, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
	
	if (c.y - a.y != 0)
	{
		inv_slope_2 = safe_inv_slope((a.x - c.x), (a.y - c.y));
		f32_clamp(&inv_slope_2, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
	
	
	if (c.y - b.y != 0)
	{
		for(s32 y = c.y; y >= b.y; --y)
		{
			s32 x_start = c.x + (y - c.y) * inv_slope_1;
			s32 x_end = c.x + (y - c.y) * inv_slope_2;
			
			if(x_end < x_start)
			{
				int_swap(&x_start, &x_end);
			}
			
			for(s32 x = x_start; x < x_end; x++)
			{
				draw_pixel_color(_color, x, y, a, b, c, engine_data);
			}
		}
	}
}

internal_f void 
fill_flat_triangle_top(triangle_t *_triangle, u32 _color, engine_shared_data_t *engine_data)
{
	// Render the upper part of the triangle (flat-bottom): from y0 to y1
	vec4_t a = _triangle->points[0];  // y0 (top)
	vec4_t b = _triangle->points[1];  // y1 (middle)
	vec4_t c = _triangle->points[2];  // y2 (bottom)	
	
	f32 inv_slope_1 = 0;
	f32 inv_slope_2 = 0;
	
	// Calculate inverse slopes (dx/dy) for left and right edges
	// Only check for division by zero
	if (b.y - a.y != 0)
	{
		inv_slope_1 = safe_inv_slope((b.x - a.x) , ((b.y - a.y)));
		
		f32_clamp(&inv_slope_1, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
	
	if (c.y - a.y != 0)
	{
		inv_slope_2 = safe_inv_slope((c.x - a.x), (c.y - a.y));
		f32_clamp(&inv_slope_2, -MAX_TEXTURED_TRIANGLE_SLOPE, MAX_TEXTURED_TRIANGLE_SLOPE);
	}
	
	
	if (b.y - a.y != 0)
	{
		for(s32 y = a.y; y <= b.y; y++)
		{
			s32 x_start = a.x + (y - a.y) * inv_slope_1;
			s32 x_end = a.x + (y - a.y) * inv_slope_2;
			
			
			if(x_end < x_start)
			{
				int_swap(&x_start, &x_end);
			}
			
			for(s32 x = x_start; x < x_end; x++)
			{
				draw_pixel_color(_color, x, y, a, b, c, engine_data);
			}
		}
	}
}

// We are using a left handed coordinate system.

global_f void 
draw_textured_triangle(triangle_t *_triangle, texture_t* _texture, engine_shared_data_t *engine_shared_data)
{
	vec4_t a = _triangle->points[0];
	vec4_t b = _triangle->points[1];
	vec4_t c = _triangle->points[2];
	
	
	// Sort the triangle verteces by y.
	
	// sort y 
	if (a.y > b.y)
	{
		f32_swap_values(&a.x, &b.x);
		f32_swap_values(&a.y, &b.y);
		f32_swap_values(&a.z, &b.z);
		f32_swap_values(&a.w, &b.w);
		
		// swap uvs
		f32_swap_values(&_triangle->texture_coords[0].u, &_triangle->texture_coords[1].u);
		f32_swap_values(&_triangle->texture_coords[0].v, &_triangle->texture_coords[1].v);
	}
	
	if (b.y > c.y)	
	{
		f32_swap_values(&b.x, &c.x);
		f32_swap_values(&b.y, &c.y);
		f32_swap_values(&b.z, &c.z);
		f32_swap_values(&b.w, &c.w);
		
		f32_swap_values(&_triangle->texture_coords[1].u, &_triangle->texture_coords[2].u);
		f32_swap_values(&_triangle->texture_coords[1].v, &_triangle->texture_coords[2].v);
	}
	
	if (a.y > b.y)
	{
		f32_swap_values(&a.x, &b.x);
		f32_swap_values(&a.y, &b.y);
		f32_swap_values(&a.z, &b.z);
		f32_swap_values(&a.w, &b.w);
        
		f32_swap_values(&_triangle->texture_coords[0].u, &_triangle->texture_coords[1].u);
		f32_swap_values(&_triangle->texture_coords[0].v, &_triangle->texture_coords[1].v);
	}
	
	
	// Flip the V componenet to account for the inverted UV-coords (V grows downwards)
	
	_triangle->texture_coords[0].v = 1 -  _triangle->texture_coords[0].v;
	_triangle->texture_coords[1].v = 1 -  _triangle->texture_coords[1].v;
	_triangle->texture_coords[2].v = 1 -  _triangle->texture_coords[2].v;
    
    
	// Update triangle with sorted vertices - keep original for barycentric interpolation
	_triangle->points[0] = a;
	_triangle->points[1] = b;
	_triangle->points[2] = c;
    
	// Render both flat-bottom and flat-top parts using the SAME original triangle
	// The barycentric interpolation in draw_texel uses the original a,b,c for correct texture mapping
	fill_triangle_bottom_textured(_triangle, _texture, engine_shared_data);
	fill_triangle_top_textured(_triangle, _texture, engine_shared_data);	
	
}

void
draw_filled_triangle(triangle_t *_triangle, u32 _color, engine_shared_data_t *engine_data)
{
	vec4_t a = _triangle->points[0];
	vec4_t b = _triangle->points[1];
	vec4_t c = _triangle->points[2];	
	
	// Sort the triangle verteces by y.
	
	// sort y 
	if (a.y > b.y)
	{
		f32_swap_values(&a.x, &b.x);
		f32_swap_values(&a.y, &b.y);
		f32_swap_values(&a.z, &b.z);
		f32_swap_values(&a.w, &b.w);		
	}
	
	if (b.y > c.y)	
	{
		f32_swap_values(&b.x, &c.x);
		f32_swap_values(&b.y, &c.y);
		f32_swap_values(&b.z, &c.z);
		f32_swap_values(&b.w, &c.w);		
	}
	
	if (a.y > b.y)
	{
		f32_swap_values(&a.x, &b.x);
		f32_swap_values(&a.y, &b.y);
		f32_swap_values(&a.z, &b.z);
		f32_swap_values(&a.w, &b.w);		
	}
	
    
	// Update triangle with sorted vertices - keep original for barycentric interpolation
	_triangle->points[0] = a;
	_triangle->points[1] = b;
	_triangle->points[2] = c;
	
	fill_flat_triangle_bottom(_triangle, _color, engine_data);
	fill_flat_triangle_top(_triangle, _color, engine_data);	
}
