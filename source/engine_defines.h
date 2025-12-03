/* date = February 21st 2025 10:14 am */

////////////////// Global  for the Engine ///////////////

typedef struct
{
	app_init* app_init;
	app_update* app_update;
	app_render* app_render;
	app_input* app_input;
	app_end* app_end;
    
#ifdef _WINDOWS
	HMODULE handle;
#endif // _WINDOWS
    
#ifdef _APPLE
    void* handle;
#endif // APPLE
    
} dynamic_app_code;

dynamic_app_code g_app_code;


typedef struct 
{
	u32 permanent;
	u32 transient;
} memory_cache_t;

global memory_cache_t g_memory_cache;
