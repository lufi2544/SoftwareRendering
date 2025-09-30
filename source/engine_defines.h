/* date = February 21st 2025 10:14 am */

////////////////// Global  for the Engine ///////////////


global bool is_running = true;

typedef struct
{
	app_init* app_init_function;
	app_update* app_update_function;
	app_render* app_render_function;
	app_end* app_end_function;
	
} win32_app_code;

win32_app_code g_app_code;