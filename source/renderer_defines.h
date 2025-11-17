/* date = February 21st 2025 10:16 am */

////////////////// Global defines for the Renderer ///////////////

#include "texture.h"
#include "texture_manager.h"
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

// Forward declarations
bool render_settings_check_flag(render_settings *_render_settings, enum_render_settings_flags flag);
void render_settings_set_flag(render_settings *_render_settings, enum_render_settings_flags flag, bool bValue);
void draw_filled_triangle(triangle_t *_triangle, u32 _color, engine_shared_data_t *engine_data);

internal_f void 
draw_textured_triangle(triangle_t *_triangle, texture_t* _texture, engine_shared_data_t *engine_shared_data);