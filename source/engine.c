
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
    
    temp_cube_create();
    
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

global u64 previous_frame_time;

function void update()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
    previous_frame_time = SDL_GetTicks();
    
    
    temp_cube_logic();         
    
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