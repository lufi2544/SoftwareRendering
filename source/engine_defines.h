/* date = February 21st 2025 10:14 am */

////////////////// Global  for the Engine ///////////////

typedef struct
{
	app_init* app_init;
	app_update* app_update;
	app_render* app_render;
	app_input* app_input;
	app_end* app_end;

	HMODULE handle;

} win32_app_code;

win32_app_code g_app_code;