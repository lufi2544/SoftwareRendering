
/////////////////// ENGINE ///////////////////
/**
* This file contains all the core engine functions for init, update and end.
*/

function bool engine_init()
{
    if(!create_window())
    {
        return false;
    }
    
    display_setup();
    
    return true;
}

function void engine_end(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);
    free(color_buffer);
    SDL_Quit();
}

function void update()
{
    
}


function s32 engine_run()
{
    if(!engine_init())
    {
        return 1;
    }
    
    while(is_running)
    {
        process_input();
        update();
        render();
    }    
    
    engine_end();
        
    return 0;
}