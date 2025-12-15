
/////////////////// ENGINE ///////////////////
/**
* This file contains all the core engine functions for init, update and end.
*/

global engine_shared_data_t g_engine_shared_data;
global mat4_t g_projection_matrix;
global u32 used_memory_when_app_init = 0;
global f32 g_delta_time = 0.0f;


// ---- WIN32 LAYER FOR APP DLL ----

internal_f void
engine_memory_init();

internal_f void
engine_camera_init();

internal_f void
initialize_engine_data()
{
	// Initialize 		
	g_engine_shared_data.b_is_engine_running = true;
	
	// render_settings
	g_engine_shared_data.settings = push_struct(&g_memory.permanent, render_settings);
	g_engine_shared_data.settings->fov = 45.0f * (PI / 180.0f);
	g_engine_shared_data.settings->aspect = (f32)g_engine_shared_data.window_width / (f32)g_engine_shared_data.window_height;
	g_engine_shared_data.settings->znear = 0.1f;
	g_engine_shared_data.settings->zfar = 200.0f;
	g_engine_shared_data.settings->flags = 0;
	
    engine_camera_init();
    
    // Light
	vec3_t light_dir = {0, 0, 1};
	global_light.direction = light_dir;
	
    g_engine_shared_data.meshes = 0;
	g_engine_shared_data.meshes_num = 0;
	g_engine_shared_data.memory = &g_memory;
	
	
	
	// input
	g_engine_shared_data.input_keyboard_flags = 0;
	g_engine_shared_data.input_mouse_flags = 0;
}


intenal_f void
engine_camera_init()
{
    
    // CAMERA
	vec3_t pos = {12, 0, -10};
	vec3_t null = {0, 0, 0}; 
	g_engine_shared_data.camera.position = pos;
	g_engine_shared_data.camera.pitch = 0;
	g_engine_shared_data.camera.yaw = 0;
	g_engine_shared_data.camera.roll = 0;
	g_engine_shared_data.camera.direction = null;
	g_engine_shared_data.camera.rotation = null;
	g_engine_shared_data.camera.rotation = null;
    
    vec3_t znear_position = {pos.x, pos.y, pos.z + g_engine_shared_data.render_settings.znear};
    vec3_t zfar_position = {pos.x, pos.y, pos.z + g_engine_shared_data.render_settings.zfar};
    
    // Camera view fustrum set up.
    // TODO separate this in to anohter function.
	g_engine_shared_data.camera.fustrum_planes[plane_top].point = pos;
	g_engine_shared_data.camera.fustrum_planes[plane_bottom].point = pos;
	g_engine_shared_data.camera.fustrum_planes[plane_right].point = pos;
	g_engine_shared_data.camera.fustrum_planes[plane_left].point = pos;
	g_engine_shared_data.camera.fustrum_planes[plane_far].point = zfar_position;
	g_engine_shared_data.camera.fustrum_planes[plane_near].point = znear_position;
    
    f32 fov_halfs = g_engine_shared_data.render_settings.fov / 2;
    
    // Camera view fustrum normals set up
    // right plane
    vec3_t left_plane_normal;
    
    left_plane_normal.x = cos(fov_halfs);
    left_plane_normal.y = 0;
    left_plane_normal.z = sin(fov_halfs);
    
    
    vec3_t right_plane_normal;
    right_plane_normal.x = -cos(fov_halfs); 
    right_plane_normal.y = 0;
    right_plane_normal.z = sin(fov_halfs);
    
    vec3_t top_plane_normal;
    top_plane_normal.x = 0;
    top_plane_normal.y = -cos(fov_halfs);
    top_plane_normal.z = sin(fov_halfs);
    
    vec3_t bottom_plane_normal;
    bottom_plane_normal.x = 0;
    bottom_plane_normal.y = cos(fov_halfs);
    bottom_plane_normal.z = sin(fov_halfs);
    
    vec3_t near_plane_normal;
    near_plane_normal.x = 0;
    near_plane_normal.y = 0;
    near_plane_normal.z = 1;
    
    vec3_t far_plane_normal;
    far_plane_normal.x = 0;
    far_plane_normal.y = 0;
    far_plane_normal.z = -1;
    
    
    g_engine_shared_data.camera.fustrum_planes[plane_top].normal = top_plane_normal;
	g_engine_shared_data.camera.fustrum_planes[plane_bottom].normal = bottom_plane_normal;
	g_engine_shared_data.camera.fustrum_planes[plane_right].normal = right_plane_normal;
	g_engine_shared_data.camera.fustrum_planes[plane_left].normal = left_plane_normal;
	g_engine_shared_data.camera.fustrum_planes[plane_far].normal = far_plane_normal;
	g_engine_shared_data.camera.fustrum_planes[plane_near].normal = near_plane_normal;
    
    
    
    
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
	
	// Memory cache dump on initialization, for later usage during hot relaod the app dll
	g_memory_cache.transient = g_memory.transient.used;
	g_memory_cache.permanent = g_memory.permanent.used;
    
	load_app_code(&g_app_code);
	
	// ---- RENDER INIT ----
	
	renderer_init(&g_engine_shared_data);
	
    
	// App layer init
	g_app_code.app_init(&g_engine_shared_data);	
	
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
    
	g_delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;
	
    previous_frame_time = SDL_GetTicks();    
}

internal_f void update(f32 dt);

internal_f void update_app_code()
{
	printf("updating the code \n");
	unload_app_code(&g_app_code);
	load_app_code(&g_app_code);
	g_app_code.app_init(&g_engine_shared_data);
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
        process_input(&g_engine_shared_data, g_delta_time);
        fix_delta_time();
        
        update(g_delta_time);
        render(&g_projection_matrix, &g_engine_shared_data);
		
		if(++counter >= refresh)
		{
			//update_app_code();
			//counter = 0;
		}
    }    
	
	g_app_code.app_end(&g_engine_shared_data);
    engine_end();
    
    return 0;
}

internal_f void
update(f32 dt)
{        
	g_app_code.app_update(&g_engine_shared_data, dt);	
}

internal_f void
engine_end(void)
{
	
	g_app_code.app_end(&g_engine_shared_data);
	
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(color_buffer_texture);    
    SDL_Quit();
	
	mayorana_shutdown();
}