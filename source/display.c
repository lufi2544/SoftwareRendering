
//////////////////// DISPLAY //////////////////

global u16 window_width;
global u16 window_height;
global bool bIsFullScreen;

global SDL_Window* window;

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


function void display_setup()
{       
    color_buffer = (u32*)malloc(sizeof(u32) * (window_width * window_height));
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

// Draws a rectangle on the screen at a certain coordinate X and Y with a Color.
function void draw_rect(u16 x, u16 y, u16 w, u16 h, u32 color)
{
    for(u16 height_index = y; height_index < y + h; ++height_index)
    {
        for(u16 width_index = x; width_index < x + w; ++width_index)
        {
            if(width_index < window_width && height_index < window_height)
            {                
                color_buffer[(height_index * window_width) + width_index] = color;
            }
        }
    }
}

function void draw_pixel(vec2_t position , u32 color)
{
    color_buffer[(window_width * ((u32)position.y)) + ((u32)(position.x))] = color;
}

global const u8 POINTS_PER_ROW = 9;
global const u32 POINTS_NUM = POINTS_NUM * POINTS_NUM * POINTS_NUM;
global vec3_t cube_points[POINTS_NUM];

// Fixed points per line cube
function void draw_points_cube()
{    
    
    const f32 coefficient = 2 / POINTS_PER_ROW;
    
    f32 x_coeff = -1;
    for(u32 x_index = 0; x_index < POINTS_NUM; ++x_index; x_coeff += coefficient)
    {
        f32 y_coeff = -1;
        for(u32 y_index = 0; y_index < POINTS_NUM; ++y_index; y_coeff += coefficient)
        {
            f32 z_coeff = -1;
            for(u32 z_index = 0; z_index < POINTS_NUM; ++z_index; z_coeff += coefficient)
            {
                vec3_t point = { x_index, y_index, z_index  };
                cube_points[x + y + z];
            }
        }
    }
    
}