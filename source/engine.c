
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


function void fix_delta_time()
{
    // Checking if between the prev frame and this one we have surpass the FRAME_TARGET_TIME
    // if we did, then we will wait some seconds.
    s32 time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    
    // If we are running to fast, then we just wait some time
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }    
    
    // If the machine is slower, then we will have to make a multi update, to catch-up
    
    previous_frame_time = SDL_GetTicks();    
}

function void update()
{        
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
        fix_delta_time();
        
        update();
        render();
    }    
    
    engine_end();
        
    return 0;
}