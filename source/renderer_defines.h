/* date = February 21st 2025 10:16 am */

////////////////// Global defines for the Renderer ///////////////

#include "texture.h"
#include "triangle.h"
#include "light_defines.h"

global SDL_Renderer* renderer;
global SDL_Texture* color_buffer_texture;

#define COLOR_BLACK 0x00000000
#define COLOR_WHITE 0xffffffff
#define COLOR_RED 0xffff0000

typedef enum 
{
	flag_display_wireframe_entirely =0,
	flag_display_wireframe_only = 1,
	flag_display_filled_triangles = 2,
	flag_display_filled_triangles_wire = 3,
	flag_back_face_culling = 4,
	
} enum_render_settings_flags ;

typedef struct
{
	u32 flags;
} render_settings;

global render_settings g_render_settings;


internal_f bool
render_settings_check_flag(enum_render_settings_flags flag)
{
	u32 mask = 1 << (u32)flag;
	
	return g_render_settings.flags & mask;
}

internal_f void
render_settings_set_flag(enum_render_settings_flags flag, bool bValue)
{
	u32 mask = (1 << (u32)flag);
	if(bValue)
	{
		g_render_settings.flags |= mask;
	}
	else
	{
		g_render_settings.flags &= (~mask);
	}
}



internal_f void
draw_pixel(vec2_t position , u32 color)
{
	if(position.x < 0 || position.y < 0 || position.x >= g_window_width|| position.y >= g_window_height)
	{
		return;
	}
	
	u32 Index = (g_window_width* ((u32)position.y)) + ((u32)(position.x));
	color_buffer[Index] = color;
}

// Draws a rectangle on the screen at a certain coordinate X and Y with a Color.
internal_f void
draw_rect(s32 x, s32 y, s32 w, s32 h, u32 color)
{
    for(s32 height_index = y; height_index < y + h; ++height_index)
    {
        for(s32 width_index = x; width_index < x + w; ++width_index)
        {
            if(width_index < g_window_width&& height_index < g_window_height)
            {                
                vec2_t pixel_position = { width_index, height_index };
                draw_pixel(pixel_position, color);
            }
        }
    }
    
}

internal_f void
draw_line(s32 x0, s32 y0, s32 x1, s32 y1, u32 _color)
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
		draw_pixel(position, _color);
		current_x += x_inc;
		current_y += y_inc;
	}	
}

internal_f void 
draw_linear_triangle(triangle_t *_triangle, u32 _color, bool bDrawDots)
{
	vec2_t position_0 = {_triangle->points[0].x, _triangle->points[0].y};
	vec2_t position_1 ={_triangle->points[1].x, _triangle->points[1].y};
	vec2_t position_2 ={_triangle->points[2].x, _triangle->points[2].y};
	
	if(bDrawDots)
	{				
		draw_rect(position_0.x, position_0.y, 5, 5, _color);			
		draw_rect(position_1.x, position_1.y, 5, 5, _color);				
		draw_rect(position_2.x, position_2.y, 5, 5, _color);
	}
	
	// A-B
	draw_line(position_0.x, position_0.y, position_1.x, position_1.y, _color);
	
	// B-C
	draw_line(position_1.x, position_1.y, position_2.x, position_2.y, _color);
	
	// C-A
	draw_line(position_2.x, position_2.y, position_0.x, position_0.y, _color);
}

internal_f void 
f32_swap_values(f32 *a, f32 *b)
{
	f32 temp = *a;
	*a = *b;
	*b = temp;
	
}

internal_f void 
fill_flat_triangle_top(triangle_t *_triangle, u32 _color)
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
		draw_line(x_start, y, x_end, y, _color);
		x_start += inv_slope_1;
		x_end += inv_slope_2;
	}
	
	//	printf("--------- \n");
	
}

internal_f void 
fill_flat_triangle_bottom(triangle_t *_triangle, u32 _color)
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
		draw_line(x_start, y, x_end, y, _color);
		x_start -= inv_slope_1;
		x_end -= inv_slope_2;
	}
	
}


internal_f void
draw_textured_triangle(triangle_t *_triangle, texture_t _texture)
{
	
}

internal_f void
draw_filled_triangle(triangle_t *_triangle, u32 _color)
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
		fill_flat_triangle_top(&bottom_triangle, _color);
		
	} 
	else if(a.y == b.y)
	{
		fill_flat_triangle_bottom(&bottom_triangle, _color);
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
		
		
		fill_flat_triangle_bottom(&bottom_triangle, _color);
		fill_flat_triangle_top(&top_triangle, _color);
	}
	
}
