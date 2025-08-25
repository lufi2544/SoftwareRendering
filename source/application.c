
//// MAIN APPLICATION PIPELINE ////


internal_f void 
app_init(memory_t *engine_memory)
{		
	// Init the meshes
	g_meshes = push_array(&engine_memory->permanent, APP_MESH_NUM, mesh_t);
	
	
	// TODO Create a better pipeline for importing meshes.
	
	// Create one cube
	mesh_t* rendered_mesh = &g_meshes[0];
	mesh_t* render_2 = &g_meshes[1];
	
	// for now let's pass the permanent memory	
	
	// TODO: Adding defalt parameters for mesh creation, maybe passing a transform? 
	mesh_t mesh = create_mesh_from_file(engine_memory, "data/lol.obj");
	
	mesh_t mesh_2 = create_mesh_from_file(engine_memory, "data/lol.obj");
	
	vec3_t position = {0, 0, 10};
	mesh.translation = position;
	
	vec3_t scale = {1, 1, 1};
	mesh.scale = scale;
	
	*rendered_mesh = mesh;
	*render_2 = mesh_2;
	
	
	render_2->translation.x = 5;
	render_2->translation.y = 5;
	render_2->translation.z = 30;
	
	
	const f32 fov_coeff = 45; // Adjust to your liking
	const f32 fov = fov_coeff * (PI / 180);
	const f32 aspect = (f32)g_window_width / (f32)g_window_height;
	const f32 znear = 0.1f;
	const f32 zfar = 200.0f;
	projection_matrix = mat4_make_perspective(fov, aspect, znear, zfar);		
	
	vec3_t light_dir = {0, 0, 1};
	global_light.direction = light_dir;
	
	vec3_t pos = {12, 0, -5};
	g_camera.position = pos;
	
}

internal_f void 
app_update(memory_t *engine_memory)
{
	mesh_t* cube = &g_meshes[0];
	cube->rotation.y += 0.01;
		
	
	//cube->translation.x += 0.01;
	//cube->translation.y += 0.01;
	
	mesh_t* other = &g_meshes[1];
	other->rotation.x += 0.01;
	
	
}

internal_f void 
app_render(memory_t *engine_memory)
{		
	// We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
	
	// We have the background being rendered first.
    clear_color_buffer(0x00000000);
	
	mesh_t* rendered_mesh = &g_meshes[0];
	mesh_render(rendered_mesh);					
	mesh_render(&g_meshes[1]);

}

internal_f void 
app_end(memory_t *engine_memory)
{
	
}