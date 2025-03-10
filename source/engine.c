
#ifdef __WINDOWS__
#include "memoryapi.h"
#endif //__WINDOWS__

/////////////////// ENGINE ///////////////////
/**
* This file contains all the core engine functions for init, update and end.
*/

internal void
engine_memory_init();

internal bool
engine_init()
{
    if(!create_window())
    {
        return false;
    }
	
	engine_memory_init();
	display_setup(&g_arena);	
	app_init(&g_arena);
    
    return true;
}

internal void 
engine_memory_init()
{
	// Use the memory arena for initialising the engine.
	// Init memory
	void* engine_memory = VirtualAlloc(NULL, Gigabyte(1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!engine_memory)
	{
		fprintf(stderr, "engine memory failed to be allocated \n");		
		return;
	}	
	
	printf("Allocated memory \n");
	
	initialize_arena(&g_arena, Gigabyte(1), engine_memory);	
}


internal void
engine_end(void);

global u64 previous_frame_time;

internal void
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

internal void update();

internal s32
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
	
	app_end(&g_arena);
    engine_end();
        
    return 0;
}

internal void
update()
{        
	app_update(&g_arena);	
}


internal void
engine_end(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);    
    SDL_Quit();
	
	if(g_arena.base != 0)
	{
		VirtualFree(g_arena.base, 0, MEM_RELEASE);
	}		
}