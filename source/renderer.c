

////////////// RENDERER //////////////


#define GRID_DEFAULT_COLOR 0xA0A0A0
void render(void)
{
    
    // TEMP: Cube logic 
    for(u32 i = 0; i < POINTS_NUM; ++i)
    {
        vec3_t point = cube_points[i];
        vec2_t projected_point = project_vec3(&point);
        
        cube_projected_points[i] = projected_point;
    }
    
    SDL_RenderClear(renderer);
    
    map_texture_to_pixels_buffer();
    
    // We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
    clear_color_buffer(0xFFFFFF);
    
    
    
    f32 x = 1000.0f;
    f32 y = 1000.0f;
    const f32 distance_coefficient = 128;
    for(u32 i = 0; i < POINTS_NUM; ++i)
    {
        vec2_t point = cube_projected_points[i];
        point.x = (point.x * distance_coefficient) + x;
        point.y = (point.y * distance_coefficient) + y;
        draw_rect(point.x, point.y,  4, 4, 0x00000000);
    }
    
    vec2_t Position = { 1000, 1000 };  
    //draw_pixel(Position, (0x00000000));
    
    SDL_RenderPresent(renderer);
}


