

////////////// RENDERER //////////////

#define GRID_DEFAULT_COLOR 0xA0A0A0



internal_f void
render(void)
{
    
    SDL_RenderClear(renderer);   
    map_texture_to_pixels_buffer();
    
    // We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
    clear_color_buffer(0xFFFFFF);
        
	app_render(&g_memory);
    
    SDL_RenderPresent(renderer);
}
