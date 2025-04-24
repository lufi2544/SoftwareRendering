/* date = February 21st 2025 10:16 am */

////////////////// Global defines for the Rendere ///////////////

#include "triangle.h"
#include "mesh.h"

global SDL_Renderer* renderer;

// (juanes.rayo): kind of renderer kind of display, but here is fine.
global u32* color_buffer;
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