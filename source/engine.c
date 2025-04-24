

#include "memoryapi.h"


/////////////////// ENGINE ///////////////////
/**
* This file contains all the core engine functions for init, update and end.
*/

internal_f void
engine_memory_init();

internal_f bool
engine_init()
{
    if(!create_window())
    {
        return false;
    }
	
	Mayorana_Framework_Init();
	
	display_setup(&g_memory);	
	app_init(&g_memory);
    
    return true;
}

internal_f void
engine_end(void);

global u64 previous_frame_time;

internal_f void
fix_delta_time()
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

internal_f void update();

internal_f s32
engine_run()
{
    if(!engine_init())
    {
		engine_end();
        return 1;
    }
    
    while(is_running)
    {
        process_input();
        fix_delta_time();
        
        update();
        render();
    }    
	
	app_end(&g_memory);
    engine_end();
        
    return 0;
}

internal_f void
update()
{        
	app_update(&g_memory);	
}


internal_f void
engine_end(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);    
    SDL_Quit();
	
	Mayorana_Framework_End();
}