#include <memoryapi.h>
#include <libloaderapi.h>

/////////////////// ENGINE ///////////////////
/**
* This file contains all the core engine functions for init, update and end.
*/

global engine_shared_data_t g_engine_shared_data;
global mat4_t g_projection_matrix;
global u32 used_memory_when_app_init = 0;

typedef struct 
{
	u32 permanent;
	u32 transient;
} memory_cache_t;

global memory_cache_t g_memory_cache;


// ---- WIN32 LAYER FOR APP DLL ----
internal_f void
win32_unload_app_code(win32_app_code *_app_code)
{
	if(_app_code->handle)
	{
		printf("REMOVING: freeing library \n");
		FreeLibrary(_app_code->handle);
		_app_code->handle = 0;
	}

	g_memory.permanent.used = g_memory_cache.permanent;
	g_memory.transient.used = g_memory_cache.transient;
}

// TODO: Reset the pointer here for the used memory by the engine
internal_f void
win32_load_app_code(win32_app_code *_app_code)
{
	
	win32_app_code result;
	
	CopyFile("app.dll", "app_temp.dll", FALSE);
	printf("OUUUUUUUUUUU: copying file\n ");
	result.handle = LoadLibraryA("app_temp.dll");
	
	if(result.handle)
	{
		result.app_init = (app_init*)GetProcAddress(result.handle, "AppInit");
		if(result.app_init)
		{
			printf("AppInit function found ...\n");
		}

		result.app_end = (app_end*)GetProcAddress(result.handle, "AppEnd");
		if(result.app_init)
		{
			printf("AppEnd function found ...\n");
		}

		result.app_update = (app_update*)GetProcAddress(result.handle, "AppUpdate");
		if(result.app_update)
		{
			printf("AppUpdate function found ... \n");
		}

		result.app_input = (app_input*)GetProcAddress(result.handle, "AppInput");
		if(result.app_input)
		{
			printf("AppInput function found ... \n");
		}

		result.app_render = (app_render*)GetProcAddress(result.handle, "AppRender");
		if(result.app_input)
		{
			printf("AppRender function found ... \n");
		}


		if(!result.app_init)
		{
			printf("Error: AppInitfunction not found ... \n");
		}

		if(!result.app_end)
		{
			printf("Error: AppEnd function not found ... \n");
		}

		if(!result.app_update)
		{
			printf("Error: AppUpdate function not found ... \n");
		}

		if(!result.app_render)
		{
			printf("Error: AppRender function not found ... \n");
		}

		if(!result.app_input)
		{
			printf("Error: AppInput function not found ... \n");
		}
	}


	*_app_code = result;
}
// ----  ----

internal_f void
engine_memory_init();

internal_f void
initialize_engine_data()
{
	// Initialize 		
	g_engine_shared_data.b_is_engine_running = true;
	
	// render_settings
	g_engine_shared_data.settings = push_struct(&g_memory.permanent, render_settings);
	g_engine_shared_data.settings->fov = 45.0f * (PI / 180.0f);
	g_engine_shared_data.settings->aspect = g_engine_shared_data.window_width / g_engine_shared_data.window_height;
	g_engine_shared_data.settings->znear = 0.1f;
	g_engine_shared_data.settings->zfar = 200.0f;
	g_engine_shared_data.settings->flags = 0;
	
	vec3_t light_dir = {0, 0, 1};
	global_light.direction = light_dir;
	
	vec3_t pos = {12, 0, -10};
	g_engine_shared_data.camera.position = pos;
	g_engine_shared_data.meshes = 0;
	g_engine_shared_data.meshes_num = 0;
}

internal_f bool
engine_init()
{
    if(!create_window(&g_engine_shared_data))
    {
        return false;
    }	
	
	// ---- MAIN ENGINE INIT ----
	mayorana_init();		
	initialize_engine_data();
	display_setup(&g_memory, &g_engine_shared_data);
	
	// Memory cache dump for later hot relaod the app dll
	g_memory_cache.transient = g_memory.transient.used;
	g_memory_cache.permanent = g_memory.permanent.used;

	win32_load_app_code(&g_app_code);


	// App layer init
	g_app_code.app_init(&g_memory, &g_engine_shared_data);	
	
	// Perspective matrixe for the engine, this is fine for now, but if we add more cameras, we would need to create a perspective matrix for each, so they would have to be separated.
	g_projection_matrix = mat4_make_perspective(g_engine_shared_data.settings->fov, g_engine_shared_data.settings->aspect, g_engine_shared_data.settings->znear, g_engine_shared_data.settings->zfar);
    
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

internal_f void update_app_code()
{
	printf("updating the code \n");
	win32_unload_app_code(&g_app_code);
	win32_load_app_code(&g_app_code);
	g_app_code.app_init(&g_memory, &g_engine_shared_data);
}

internal_f s32
engine_run()
{
    if(!engine_init())
    {
		engine_end();
        return 1;
    }
    
	u32 counter = 0;
	u32 refresh = 120;
	
    while(g_engine_shared_data.b_is_engine_running)
    {
        process_input(&g_engine_shared_data);
        fix_delta_time();
        
        update();
        render(&g_projection_matrix, &g_memory, &g_engine_shared_data);
		
		if(++counter >= refresh)
		{
			//update_app_code();
			//counter = 0;
		}
    }    
	
	g_app_code.app_end(&g_memory, &g_engine_shared_data);
    engine_end();
        
    return 0;
}

internal_f void
update()
{        
	g_app_code.app_update(&g_memory, &g_engine_shared_data);	
}

internal_f void
engine_end(void)
{
	
	g_app_code.app_end(&g_memory, &g_engine_shared_data);
	
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);    
    SDL_Quit();
	
	mayorana_shutdown();
}