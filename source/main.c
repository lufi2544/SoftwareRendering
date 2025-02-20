
#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <types.h>

bool is_running = true;


u16 window_width = 0;
u16 window_height = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool create_window(void)
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

u32* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

void setup()
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

void process_input()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    
    switch(event.type)
    {
        case SDL_QUIT:
        {
            is_running = false;
        }
        break;
        
        case SDL_KEYDOWN:
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                is_running = false;
            }
        }
        break;
        
        default:
        break;
    }
}

void update(void)
{
    
}


// Maps the SDL_texture to the pixels buffer and pass the Texture to the renderer.
void map_texture_to_pixels_buffer(void)
{
    SDL_UpdateTexture(
                      color_buffer_texture,
                      NULL,
                      color_buffer,
                      (int)(window_width * sizeof(u32))
                      );
    

    
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(u32 color)
{
    for(u16 height_index = 0; height_index < window_height; height_index++)
    {
        for(u16 width_index = 0; width_index < window_width; width_index++)
        {
            color_buffer[width_index + (height_index * window_width)] = color;
        }
    }
}

void draw_grid(u16 square_size, u32 color)
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
void draw_rect(u16 x, u16 y, u16 w, u16 h, u32 color)
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


#define GRID_DEFAULT_COLOR 0xA0A0A0
void render(void)
{
    SDL_RenderClear(renderer);
       
    map_texture_to_pixels_buffer();
    
    // We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
    clear_color_buffer(0xFFFFFF);
    draw_rect(100, 100, 500, 500, GRID_DEFAULT_COLOR);
    
    SDL_RenderPresent(renderer);
}



void end(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);
    free(color_buffer);
    SDL_Quit();
}

int main(int argc, char* argv[])
{    
    if(!create_window())
    {
        return 1;
    }
    
    setup();    
    while(is_running)
    {
        process_input();
        update();
        render();
    }    
    
    end();
    
    return 0;
}