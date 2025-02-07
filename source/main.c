
#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <types.h>

bool is_running = true;


u16 window_width = 800;
u16 window_height = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool create_window(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL \n");
        return false;
    }
    
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
    }
    
    
    color_buffer_texture = SDL_CreateTexture(
                                             renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height
                                             );
    
    if(color_buffer_texture)
    {
        printf("Texture Created \n");
    }

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


void render_color_buffer(void)
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

void render(void)
{
   // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    render_color_buffer();
    clear_color_buffer(0xFFFFFF00);
    
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
    printf("Hello World \n");
    
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