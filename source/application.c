
//// MAIN APPLICATION PIPELINE ////

internal void 
app_init(engine_memory_t *engine_memory)
{	
	TEMP_MEMORY();	
	string_t hello = STRING_V(temp_arena, "Hello");	
	printf("This is the string: %s \n", STRING_CONTENT(hello));
	TEMP_MEMORY_END();	
	
	
	// Init the meshes
	g_meshes = PushArray(&engine_memory->permanent, APP_MESH_NUM, mesh_t);
		
	// Create one cube
	mesh_t* cube = &g_meshes[0];
	
	// for now let's pass the permanent memory
	cube_mesh_init(cube, &engine_memory->permanent);
	
	mesh_t mesh = create_mesh_from_file(engine_memory, "data/lol.obj");
}

internal void 
app_update(engine_memory_t *engine_memory)
{
	mesh_t* cube = &g_meshes[0];
	cube_mesh_update(cube);
}

internal void 
app_render(engine_memory_t *engine_memory)
{	
	//	temp_cube_render();
	mesh_t* cube = &g_meshes[0];
	cube_mesh_render(cube);
}

internal void 
app_end(engine_memory_t *engine_memory)
{
	
}