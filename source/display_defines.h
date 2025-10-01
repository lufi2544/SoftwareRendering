/* date = February 28th 2025 9:55 am */

#include "display_config.h"

/////// DISPLAY DATA ///////
global u16 g_window_width;
global u16 g_window_height;
global bool bIsFullScreen;

global SDL_Window* window;

global u32* color_buffer;

internal_f void clear_color_buffer(u32 color)
{
    for(u16 height_index = 0; height_index < g_window_height; height_index++)
    {
        for(u16 width_index = 0; width_index < g_window_width; width_index++)
        {
            color_buffer[width_index + (height_index * g_window_width)] = color;
        }
    }
}