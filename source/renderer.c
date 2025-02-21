

////////////// RENDERER //////////////

#define GRID_DEFAULT_COLOR 0xA0A0A0
void render(void)
{
    SDL_RenderClear(renderer);
    
    map_texture_to_pixels_buffer();
    
    // We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
    clear_color_buffer(0xFFFFFF);
    draw_rect(50, 10, 500, 500, GRID_DEFAULT_COLOR);
    
    
    vec2_t Position = { 1000, 1000 };  
    draw_pixel(Position, (0x00000000));
    
    SDL_RenderPresent(renderer);
}


