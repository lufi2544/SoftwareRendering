#include <dlfcn.h>
#include <copyfile.h>

internal_f void
apple_unload_app_code(dynamic_app_code *_app_code)
{
	if(_app_code->handle)
	{
		printf("REMOVING: freeing library \n");
		dlclose(_app_code->handle);
		_app_code->handle = 0;
	}
    
	g_memory.permanent.used = g_memory_cache.permanent;
	g_memory.transient.used = g_memory_cache.transient;
}

// TODO: Reset the pointer here for the used memory by the engine
internal_f void
apple_load_app_code(dynamic_app_code *_app_code)
{ 
	
	dynamic_app_code result;
    
    copyfile_state_t s;
    s = copyfile_state_alloc();
    copyfile("app.dylib", "app_temp.dylib", s, COPYFILE_DATA | COPYFILE_XATTR);
    copyfile_state_free(s);
    
    
    result.handle = dlopen("app_temp.dylib", RTLD_NOW);
	
	if(result.handle)
	{
		result.app_init = (app_init*)dlsym(result.handle, "AppInit");
		if(result.app_init)
		{
			printf("AppInit function found ...\n");
		}
        
		result.app_end = (app_end*)dlsym(result.handle, "AppEnd");
		if(result.app_init)
		{
			printf("AppEnd function found ...\n");
		}
        
		result.app_update = (app_update*)dlsym(result.handle, "AppUpdate");
		if(result.app_update)
		{
			printf("AppUpdate function found ... \n");
		}
        
		result.app_input = (app_input*)dlsym(result.handle, "AppInput");
		if(result.app_input)
		{
			printf("AppInput function found ... \n");
		}
        
		result.app_render = (app_render*)dlsym(result.handle, "AppRender");
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