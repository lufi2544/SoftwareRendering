

////////////// RENDERER //////////////

#define GRID_DEFAULT_COLOR 0xA0A0A0



internal_f void
render(void)
{
    
    SDL_RenderClear(renderer);   
    map_texture_to_pixels_buffer();    
        
	g_app_code.app_render(&g_memory);
    
    SDL_RenderPresent(renderer);
}
