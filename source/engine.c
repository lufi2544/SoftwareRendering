#include <memoryapi.h>
#include <libloaderapi.h>

/////////////////// ENGINE ///////////////////
/**
* This file contains all the core engine functions for init, update and end.
*/

global engine_shared_data_t g_engine_shared_data;
global mat4_t g_projection_matrix;

internal_f win32_app_code
win32_load_app_code(void)
{
	win32_app_code result;
	result.app_init = AppInitStub;
	result.app_update = AppUpdateStub;
	result.app_render = AppRenderStub;
	result.app_input = AppInputStub;
	result.app_end = AppEndStub;
	
	HMODULE app_code_dll = LoadLibraryA("app.dll");
	if(app_code_dll)
	{
		result.app_init = (app_init*)GetProcAddress(app_code_dll, "AppInit");
		if(result.app_init)
		{
			printf("AppInit function found ...\n");
		}
		
		result.app_end = (app_end*)GetProcAddress(app_code_dll, "AppEnd");
		if(result.app_init)
		{
			printf("AppEnd function found ...\n");
		}
		
		result.app_update = (app_update*)GetProcAddress(app_code_dll, "AppUpdate");
		if(result.app_update)
		{
			printf("AppUpdate function found ... \n");
		}
		
		result.app_input = (app_input*)GetProcAddress(app_code_dll, "AppInput");
		if(result.app_input)
		{
			printf("AppInput function found ... \n");
		}
		
		result.app_render = (app_render*)GetProcAddress(app_code_dll, "AppRender");
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
	
	return result;
}

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
	g_app_code = win32_load_app_code();
	initialize_engine_data();
	// ---- ----		
	display_setup(&g_memory, &g_engine_shared_data);	
	g_app_code.app_init(&g_memory, &g_engine_shared_data);
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

internal_f s32
engine_run()
{
    if(!engine_init())
    {
		engine_end();
        return 1;
    }
    
    while(g_engine_shared_data.b_is_engine_running)
    {
        process_input(&g_engine_shared_data);
        fix_delta_time();
        
        update();
        render(&g_projection_matrix, &g_memory, &g_engine_shared_data);
		
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
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);    
    SDL_Quit();
	
	mayorana_shutdown();
}