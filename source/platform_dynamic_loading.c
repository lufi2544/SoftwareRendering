
#ifdef _APPLE
#include "apple_dynamic_code_loading.c"
#endif //_APPLE

#ifdef _WINDOWS
#include "win32_dynamic_code_loading.c"
#endif // _WINDOWS




global_f void 
load_app_code(dynamic_app_code *code)
{
    
#ifdef _APPLE
    apple_load_app_code(code);
#endif // _APPLE
    
#ifdef _WINDOWS
    win32_load_app_code(code);
#endif //_WINDOWS
}


global_f void
unload_app_code(dynamic_app_code *code)
{
    
#ifdef _APPLE
	
    apple_unload_app_code(code);
	
#endif // _APPLE	
    
#ifdef _WINDOWS
	
    win32_unload_app_code(code);
	
#endif //_WINDOWS
}