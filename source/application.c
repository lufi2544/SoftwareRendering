
//// MAIN APPLICATION PIPELINE ////

internal void 
app_init(engine_memory_t *engine_memory)
{		
	// Init the meshes
	g_meshes = PushArray(&engine_memory->permanent, APP_MESH_NUM, mesh_t);
		
	// Create one cube
	mesh_t* rendered_mesh = &g_meshes[0];
	
	// for now let's pass the permanent memory	
	
	mesh_t mesh = create_mesh_from_file(engine_memory, "data/cubito_0.obj");		
	*rendered_mesh = mesh;
}

internal void 
app_update(engine_memory_t *engine_memory)
{
	mesh_t* cube = &g_meshes[0];
}

internal void 
app_render(engine_memory_t *engine_memory)
{	
	mesh_t* rendered_mesh = &g_meshes[0];
	mesh_render(rendered_mesh);
	

}

internal void 
app_end(engine_memory_t *engine_memory)
{
	
}