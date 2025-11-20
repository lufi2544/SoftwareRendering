/* date = March 7th 2025 11:12 am */


#define APP_MESH_NUM 10

#define APP_INIT(name) void name(engine_shared_data_t* shared_data)
typedef APP_INIT(app_init);
APP_INIT(AppInitStub)
{
	
}

#define APP_UPDATE(name) void name(engine_shared_data_t* shared_data)
typedef APP_UPDATE(app_update);
APP_UPDATE(AppUpdateStub)
{
	
}

#define APP_RENDER(name) void name(engine_shared_data_t* shared_data)
typedef APP_RENDER(app_render);
APP_RENDER(AppRenderStub)
{
	
}

#define APP_INPUT(name) void name(engine_shared_data_t* shared_data)
typedef APP_INPUT(app_input);
APP_INPUT(AppInputStub)
{
	
}

#define APP_END(name) void name(engine_shared_data_t* shared_data)
typedef APP_END(app_end);
APP_END(AppEndStub)
{
	
}