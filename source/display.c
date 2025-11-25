
//////////////////// DISPLAY //////////////////

internal_f bool create_window(engine_shared_data_t *shared_data)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL \n");
        return false;
    }
    
    // Using SDL to query the full width and height of the display.
    SDL_DisplayMode display_mode;    
    SDL_GetCurrentDisplayMode(0, &display_mode);
    
    shared_data->window_width= display_mode.w;
    shared_data->window_height = display_mode.h;
	
    
    window = SDL_CreateWindow(
                              
                              NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              shared_data->window_width,
                              shared_data->window_height,
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
    
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    return true;
}

internal_f void display_setup(memory_t *engine_memory, engine_shared_data_t *shared_data)
{          
	color_buffer = push_array(&engine_memory->permanent, (shared_data->window_width* shared_data->window_height), u32);
	
	z_buffer = push_array(&engine_memory->permanent, (shared_data->window_width*
 shared_data->window_height), f32);
    
    color_buffer_texture = SDL_CreateTexture(
                                             renderer,
                                             SDL_PIXELFORMAT_RGBA32,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             shared_data->window_width,
                                             shared_data->window_height
                                             );
    
       
}

// Maps the SDL_texture to the pixels buffer and pass the Texture to the renderer.
internal_f void map_texture_to_pixels_buffer(engine_shared_data_t* engine_data)
{
    SDL_UpdateTexture(
                      color_buffer_texture,
                      NULL,
                      color_buffer,
                      (int)(engine_data->window_width * sizeof(u32))
                      );
    
    
    
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}


///////////////////// HELPER ///////////////////// 

internal_f void draw_grid(u16 square_size, u32 color, engine_shared_data_t *engine_data)
{
    for(u16 height_index = 0; height_index < engine_data->window_height; height_index++)
    {
        for(u16 width_index = 0; width_index < engine_data->window_width; width_index++)
        {
            if(height_index % square_size == 0 || width_index % square_size == 0)
            {
                color_buffer[width_index + (height_index * engine_data->window_width)] = color;
            }
        }
    }
}


internal_f void
clear_color_buffer(u32 color, engine_shared_data_t *engine_data)
{
    for(u16 height_index = 0; height_index < engine_data->window_height; height_index++)
    {
        for(u16 width_index = 0; width_index < engine_data->window_width; width_index++)
        {
            color_buffer[width_index + (height_index * engine_data->window_width)] = color;
        }
    }
}

internal_f void
clear_z_buffer(engine_shared_data_t *engine_data)
{
	for(u16 height_idx = 0; height_idx < engine_data->window_height; height_idx++)
	{
		for(u16 width_idx = 0; width_idx < engine_data->window_width; width_idx++)
		{
			z_buffer[(height_idx * engine_data->window_width) + width_idx] = 1.0f;
		}
	}
}
