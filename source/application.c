
//// MAIN APPLICATION PIPELINE ////

internal void app_init(memory_arena_t *_arena)
{
	//fill_cube_points();
	
	
	// Init the meshes
	g_meshes = PushArray(_arena, APP_MESH_NUM, mesh_t);
		
	// Create one cube
	mesh_t* cube = &g_meshes[0];		
	cube_mesh_init(cube, _arena);
}

internal void app_update(memory_arena_t *_arena)
{
	mesh_t* cube = &g_meshes[0];
	cube_mesh_update(cube);
}

internal void app_render(memory_arena_t *_arena)
{	
	//	temp_cube_render();
	mesh_t* cube = &g_meshes[0];
	cube_mesh_render(cube);
}

internal void app_end(memory_arena_t *_arena)
{
	
}