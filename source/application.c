
//// MAIN APPLICATION PIPELINE ////

APP_INIT(AppInit)
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
	
	
	const f32 fov_coeff = 45;
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

APP_UPDATE(AppUpdate)
{
	mesh_t* cube = &g_meshes[0];
	cube->rotation.y += 0.01;
		
	
	//cube->translation.x += 0.01;
	//cube->translation.y += 0.01;
	
	mesh_t* other = &g_meshes[1];
	other->rotation.x += 0.01;
	
	
}

APP_RENDER(AppRender)
{		
	// We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
	
	// We have the background being rendered first.
    clear_color_buffer(0x00000000);
	
	mesh_t* rendered_mesh = &g_meshes[0];
	mesh_render(engine_memory, rendered_mesh);					
	mesh_render(engine_memory, &g_meshes[1]);

}

APP_INPUT(AppInput)
{
	SDL_Event event;
    SDL_PollEvent(&event);
    
    switch(event.type)
    {
        case SDL_QUIT:
        {
            is_running = false;
        }
        break;
        
        case SDL_KEYDOWN:
        {
			SDL_Keycode key = event.key.keysym.sym;
            if(key == SDLK_ESCAPE)
            {
                is_running = false;
            }
			else if(key == SDLK_UP)
			{
				
				g_camera.position.y += 0.1;
				
			}
			else if(key == SDLK_DOWN)
			{
				g_camera.position.y -= 0.1;
			}
			else if(key == SDLK_RIGHT)
			{
				g_camera.position.x += 0.1;
			}
			else if(key == SDLK_LEFT)
			{
				g_camera.position.x -= 0.1;
			}
			else if(key == 'e')
			{
				g_camera.position.z += 0.1;
			}
			else if(key == 'd')
			{
				g_camera.position.z -= 0.1;
			}
			else if(key == SDLK_1)
			{
				render_settings_set_flag(flag_display_wireframe_only, false);
				
				bool bValue = !render_settings_check_flag(flag_display_wireframe_entirely);				
				render_settings_set_flag(flag_display_wireframe_entirely, bValue);
			}
			else if(key == SDLK_2)
			{
				
				render_settings_set_flag(flag_display_wireframe_entirely, false);
				
				bool bValue = !render_settings_check_flag(flag_display_wireframe_only);
				render_settings_set_flag(flag_display_wireframe_only, bValue);					
				
				
			}
			else if(key == SDLK_3)
			{
				bool bValue = !render_settings_check_flag(flag_display_filled_triangles);
				render_settings_set_flag(flag_display_filled_triangles, bValue);
			}
			else if(key == SDLK_4)
			{
				bool bValue1 = !render_settings_check_flag(flag_display_filled_triangles);
				bool bValue2 = !render_settings_check_flag(flag_display_wireframe_only);
				render_settings_set_flag(flag_display_filled_triangles, bValue1);				
				render_settings_set_flag(flag_display_wireframe_only, bValue2);
				
			}
			else if(key == SDLK_c)
			{
				bool bValue = !render_settings_check_flag(flag_back_face_culling);											
				render_settings_set_flag(flag_back_face_culling, bValue);
			}
			
		}
		break;
		default:
		break;
	}
}

APP_END(AppEnd)
{
	
}