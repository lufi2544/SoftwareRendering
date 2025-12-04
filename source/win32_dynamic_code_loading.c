
#include <memoryapi.h>
#include <libloaderapi.h>

internal_f void
win32_unload_app_code(dynamic_app_code *_app_code)
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
win32_load_app_code(dynamic_app_code *_app_code)
{
	
	dynamic_app_code result;
	
	CopyFile("app.dll", "app_temp.dll", FALSE);
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
