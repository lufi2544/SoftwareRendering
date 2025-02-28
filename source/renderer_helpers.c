
function void draw_pixel(vec2_t *position , u32 color)
{
    color_buffer[(window_width * ((u32)position->y)) + ((u32)(position->x))] = color;
}

// Draws a rectangle on the screen at a certain coordinate X and Y with a Color.
function void draw_rect(u16 x, u16 y, u16 w, u16 h, u32 color)
{
    for(u16 height_index = y; height_index < y + h; ++height_index)
    {
        for(u16 width_index = x; width_index < x + w; ++width_index)
        {
            if(width_index < window_width && height_index < window_height)
            {                
                vec2_t pixel_position = { width_index, height_index };
                draw_pixel(&pixel_position, color);
            }
        }
    }
    
}