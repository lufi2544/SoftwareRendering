

#include "memoryapi.h"


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
	display_setup(&g_engine_memory);	
	app_init(&g_engine_memory);
    
    return true;
}

internal void 
engine_memory_init()
{
	// Use the memory arena for initialising the engine.
	// Init memory
	void* engine_memory_permanent = VirtualAlloc(NULL, ENGINE_MEMORY_PERMANENT_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!engine_memory_permanent)
	{
		fprintf(stderr, "permanent engine memory failed to be allocated \n");		
		return;
	}
	
	void* engine_memory_transient = VirtualAlloc(0, ENGINE_MEMORY_TRANSIENT_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!engine_memory_transient)
	{
		fprintf(stderr, "transient engine memory failed to be allocated \n");		
		return;
	}
		
	initialize_arena(&g_engine_memory.permanent, ENGINE_MEMORY_PERMANENT_SIZE, engine_memory_permanent);	
	initialize_arena(&g_engine_memory.transient, ENGINE_MEMORY_TRANSIENT_SIZE, engine_memory_transient);	
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
	
	app_end(&g_engine_memory);
    engine_end();
        
    return 0;
}

internal void
update()
{        
	app_update(&g_engine_memory);	
}


internal void
engine_end(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);    
    SDL_Quit();
	
	if (g_engine_memory.permanent.base)
	{
		VirtualFree(g_engine_memory.permanent.base, 0, MEM_RELEASE);
	}
	
	if (g_engine_memory.transient.base)
	{
		VirtualFree(g_engine_memory.transient.base, 0, MEM_RELEASE);
	}	
}