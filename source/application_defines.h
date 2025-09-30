/* date = March 7th 2025 11:12 am */

#define APP_MESH_NUM 10


// TODO figure out where to store this
mat4_t projection_matrix;


// TODO platform Add return values maybe?

#define APP_INIT(name) void name(memory_t *engine_memory)
typedef APP_INIT(app_init);
APP_INIT(AppInitStub)
{
	
}

#define APP_UPDATE(name) void name(memory_t *engine_memory)
typedef APP_UPDATE(app_update);
APP_UPDATE(AppUpdateStub)
{
	
}

#define APP_RENDER(name) void name(memory_t *engine_memory)
typedef APP_RENDER(app_render);
APP_RENDER(AppRenderStub)
{
	
}


#define APP_END(name) void name(memory_t *engine_memory)
typedef APP_END(app_end);
APP_RENDER(AppEndStub)
{
	
}