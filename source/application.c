
//// MAIN APPLICATION PIPELINE ////

internal void 
app_init(engine_memory_t *engine_memory)
{		
	// Init the meshes
	g_meshes = PushArray(&engine_memory->permanent, APP_MESH_NUM, mesh_t);
		
	// Create one cube
	mesh_t* rendered_mesh = &g_meshes[0];
	
	// for now let's pass the permanent memory	
	
	mesh_t mesh = create_mesh_from_file(engine_memory, "data/monkey.obj");		
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
	
	
	/*triangle_t t;
	t.points[0].x = 1000;
	t.points[0].y = 500;
	
	t.points[1].x = 2000;
	t.points[1].y = 100;
	
	t.points[2].x = 600;
	t.points[2].y = 300;
	
	draw_filled_triangle(&t,COLOR_BLACK);
	*/

}

internal void 
app_end(engine_memory_t *engine_memory)
{
	
}