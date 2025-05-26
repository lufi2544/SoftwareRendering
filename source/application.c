
//// MAIN APPLICATION PIPELINE ////


internal_f void 
app_init(memory_t *engine_memory)
{		
	// Init the meshes
	g_meshes = push_array(&engine_memory->permanent, APP_MESH_NUM, mesh_t);
		
	// Create one cube
	mesh_t* rendered_mesh = &g_meshes[0];
	
	// for now let's pass the permanent memory	
	
	// TODO: Adding defalt parameters for mesh creation, maybe passing a transform? 
	mesh_t mesh = create_mesh_from_file(engine_memory, "data/monkey.obj");
	
	vec3_t position = {0, 0, 10};//{ g_window_width/2, window_height /2, 10 };
	mesh.translation = position;
	vec3_t scale = {0.5, 0.5, 0.5};
	mesh.scale = scale;
	
	*rendered_mesh = mesh;			
	
	
	const f32 fov = PI / 2.0f;
	const f32 aspect =  (f32)g_window_height / (f32)g_window_width;
	const f32 znear = 0.1;
	const f32 zfar = 100.0f;
	projection_matrix = mat4_make_perspective(fov, aspect, znear, zfar);
	
}

internal_f void 
app_update(memory_t *engine_memory)
{
	mesh_t* cube = &g_meshes[0];
	if(cube->scale.x < 2)
	{
		cube->scale.x += 0.01;
		cube->scale.y += 0.01;
		cube->scale.z += 0.01;		
	}
	
//	cube->translation.x += 0.01;
	
	cube->rotation.y += 0.01;
	
	cube->translation.x += 0.01;
	cube->translation.y -= 0.01;
	//cube->translation.z += 0.01;
	
	
}

internal_f void 
app_render(memory_t *engine_memory)
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

internal_f void 
app_end(memory_t *engine_memory)
{
	
}