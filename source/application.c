
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
	mesh_t mesh = create_mesh_from_file(engine_memory, "data/cubito_0.obj");
	
	vec3_t position = {0, 0, 10};
	mesh.translation = position;
	vec3_t scale = {1, 1, 1};
	mesh.scale = scale;
	
	*rendered_mesh = mesh;			
	
	
	const f32 fov = M_PI / 2; // Adjust to your liking
	const f32 aspect = (f32)g_window_width / (f32)g_window_height;
	const f32 znear = 0.1f;
	const f32 zfar = 100.0f;
	projection_matrix = mat4_make_perspective(fov, aspect, znear, zfar);		
	
	vec3_t light_dir = {0, 0, 1};
	global_light.direction = light_dir;
	
}

internal_f void 
app_update(memory_t *engine_memory)
{
	mesh_t* cube = &g_meshes[0];
	
	
	
	cube->rotation.y += 0.01;
	
	//cube->translation.x += 0.01;
	//cube->translation.y += 0.01;
	
	
}

internal_f void 
app_render(memory_t *engine_memory)
{		
	// We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
	
	// We have the background being rendered first.
    clear_color_buffer(0x00000000);
	
	mesh_t* rendered_mesh = &g_meshes[0];
	mesh_render(rendered_mesh);			

}

internal_f void 
app_end(memory_t *engine_memory)
{
	
}