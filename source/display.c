
//////////////////// DISPLAY //////////////////

function bool create_window(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL \n");
        return false;
    }
    
    // Using SDL to query the full width and height of the display.
    SDL_DisplayMode display_mode;    
    SDL_GetCurrentDisplayMode(0, &display_mode);
    
    window_width = display_mode.w;
    window_height = display_mode.h;
    
    window = SDL_CreateWindow(
                              
                              NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              window_width,
                              window_height,
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

function void display_setup(memory_arena_t *_arena)
{       
    color_buffer = PushArray(_arena, (window_width * window_height), u32);
    if(!color_buffer)
    {
        fprintf(stderr, "Error allocating the color_buffer.");
        return;
    }        
    
    color_buffer_texture = SDL_CreateTexture(
                                             renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height
                                             );
    
       
}

// Maps the SDL_texture to the pixels buffer and pass the Texture to the renderer.
function void map_texture_to_pixels_buffer(void)
{
    SDL_UpdateTexture(
                      color_buffer_texture,
                      NULL,
                      color_buffer,
                      (int)(window_width * sizeof(u32))
                      );
    
    
    
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}


///////////////////// HELPER ///////////////////// 
function void clear_color_buffer(u32 color)
{
    for(u16 height_index = 0; height_index < window_height; height_index++)
    {
        for(u16 width_index = 0; width_index < window_width; width_index++)
        {
            color_buffer[width_index + (height_index * window_width)] = color;
        }
    }
}

function void draw_grid(u16 square_size, u32 color)
{
    for(u16 height_index = 0; height_index < window_height; height_index++)
    {
        for(u16 width_index = 0; width_index < window_width; width_index++)
        {
            if(height_index % square_size == 0 || width_index % square_size == 0)
            {
                color_buffer[width_index + (height_index * window_width)] = color;
            }
        }
    }
}