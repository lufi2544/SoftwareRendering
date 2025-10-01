
//////////////////// DISPLAY //////////////////

internal_f bool create_window(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL \n");
        return false;
    }
    
    // Using SDL to query the full width and height of the display.
    SDL_DisplayMode display_mode;    
    SDL_GetCurrentDisplayMode(0, &display_mode);
    
    g_window_width= display_mode.w;
    g_window_height = display_mode.h;
    
    window = SDL_CreateWindow(
                              
                              NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              g_window_width,
                              g_window_height,
                              SDL_WINDOW_BORDERLESS
                              );
    
    
    if(!window)
    {
        fprintf(stderr, "Error Creating the window \n");
        return false;
    }
    
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        fprintf(stderr, "Error creating the renderer");
        return false;
    }
    
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    
    return true;
}

internal_f void display_setup(memory_t *engine_memory)
{       
    color_buffer = push_array(&engine_memory->permanent, (g_window_width* g_window_height), u32);
    if(!color_buffer)
    {
        fprintf(stderr, "Error allocating the color_buffer.");
        return;
    }        
    
    color_buffer_texture = SDL_CreateTexture(
                                             renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             g_window_width,
                                             g_window_height
                                             );
    
       
}

// Maps the SDL_texture to the pixels buffer and pass the Texture to the renderer.
internal_f void map_texture_to_pixels_buffer(void)
{
    SDL_UpdateTexture(
                      color_buffer_texture,
                      NULL,
                      color_buffer,
                      (int)(g_window_width* sizeof(u32))
                      );
    
    
    
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}


///////////////////// HELPER ///////////////////// 

internal_f void draw_grid(u16 square_size, u32 color)
{
    for(u16 height_index = 0; height_index < g_window_height; height_index++)
    {
        for(u16 width_index = 0; width_index < g_window_width; width_index++)
        {
            if(height_index % square_size == 0 || width_index % square_size == 0)
            {
                color_buffer[width_index + (height_index * g_window_width)] = color;
            }
        }
    }
}