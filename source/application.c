
//// MAIN APPLICATION PIPELINE ////


internal void app_init(memory_arena_t *_arena)
{
	//fill_cube_points();
}

internal void app_update(memory_arena_t *_arena)
{
	//temp_cube_create();
	cube_mesh_create();
}

internal void app_render(memory_arena_t *_arena)
{	
	//	temp_cube_render();
	cube_mesh_render();
}

internal void app_end(memory_arena_t *_arena)
{
	
}