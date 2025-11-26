
//// MAIN APPLICATION PIPELINE ////

// TODO Application globals

/**
* NOTE: We are using the ARGB8888 pixel format for this project, so the pixels have 32 bits of information.
*/

APP_INIT(AppInit)
{
	S_SCRATCH(shared_data->memory);
	
	// Init the meshes
	shared_data->meshes = push_array(&shared_data->memory->permanent, APP_MESH_NUM, mesh_t);
	shared_data->meshes_num = 0;
	
	
	// TODO Create a better pipeline for importing meshes.
	
	// Create one cube
	mesh_t* rendered_mesh = &shared_data->meshes[0];
	mesh_t* render_2 = &shared_data->meshes[1];		
	
	
	/*   
    TEXTURES IMPORT

	TODO: I would allocate the memory here for the different textures from the png files.
		 either we :
			 - reserve all the textures for the used textures in the game
			 - having that at runtime and reseting that at will.

    Allocate the textures in a permanent memory space.	
	 */
	upng_t* drone_png = upng_new_from_file(temp_arena, "data/drone.png");
	string_t name_drone_texture = STRING_V(temp_arena, "drone");
	if (drone_png != 0)
	{
		upng_decode(shared_data, drone_png);
		texture_t texture;
		texture.buffer = (color_t*)drone_png->buffer;
		texture.w = drone_png->width;
		texture.h = drone_png->height;		
		
		texture_manager_add_texture(&shared_data->texture_manager, name_drone_texture, texture);
	}
	
	upng_t* f117_png = upng_new_from_file(temp_arena, "data/f117.png");
	string_t f117_name = STRING_V(temp_arena, "f117");
	if (f117_png != 0)
	{
		upng_decode(shared_data, f117_png);
		texture_t texture;
		texture.buffer = (color_t*)f117_png->buffer;
		texture.w = f117_png->width;
		texture.h = f117_png->height;
		
		
		texture_manager_add_texture(&shared_data->texture_manager, f117_name, texture);
	}
	
	// for now let's pass the permanent memory
	
	// TODO: Adding defalt parameters for mesh creation, maybe passing a transform? 
	mesh_t mesh = create_mesh_from_file("data/drone.obj", shared_data);
	mesh_add_texture(shared_data, &mesh, name_drone_texture);
	
	int x = 12;
	int y = 2;
	int f = 5;
	
	vec3_t position = {x, y, f};
	mesh.translation = position;
	
	vec3_t scale = {1, 1, 1};
	mesh.scale = scale;
	
	*rendered_mesh = mesh;
			
	mesh_t mesh_ = create_mesh_from_file("data/f117.obj", shared_data);
	mesh_add_texture(shared_data, &mesh_, f117_name);
	
	
	vec3_t position_ = {x + 3, y + 3, f + 3};
	mesh_.translation = position_;
	
	vec3_t scale_ = {1, 1, 1};
	mesh_.scale = scale_;
	
	*render_2 = mesh_;
	
	
	/*
	mesh_t mesh_2 = create_mesh_from_file(engine_memory, "data/lol.obj", shared_data);
	
	printf("LOLETE %i", f);
	
	
	

	*render_2 = mesh_2;

	render_2->translation.x = 5;
	render_2->translation.y = 5;
	render_2->translation.z = 10;
*/
	
	SCRATCH_END();	
}


// TODO I think the light color and every thing that the user can twick should be exposed to the app layer, so the engine can 
// process it, but the app can modify it.
APP_UPDATE(AppUpdate)
{
 	mesh_t* cube = &shared_data->meshes[0];
	cube->rotation.y += 0.01;
 	mesh_t* cube_ = &shared_data->meshes[1];
	cube_->rotation.y += 0.01;
	
	/*
	cube->rotation.z += 0.01;
*/
	
}

APP_RENDER(AppRender)
{
	// Rendering is now handled by the engine layer
	// See renderer.c render() function
}

APP_INPUT(AppInput)
{
	SDL_Event event;
    SDL_PollEvent(&event);
    
    switch(event.type)
    {
        case SDL_QUIT:
        {
			shared_data->b_is_engine_running = false;
        }
        break;
        
        case SDL_KEYDOWN:
        {
			SDL_Keycode key = event.key.keysym.sym;
            if(key == SDLK_ESCAPE)
            {
				shared_data->b_is_engine_running = false;
            }
			else if(key == SDLK_UP)
			{
				
				shared_data->camera.position.y += 0.1;
				
			}
			else if(key == SDLK_DOWN)
			{
				shared_data->camera.position.y -= 0.1;
			}
			else if(key == SDLK_RIGHT)
			{
				shared_data->camera.position.x += 0.1;
			}
			else if(key == SDLK_LEFT)
			{
				shared_data->camera.position.x -= 0.1;
			}
			else if(key == 'e')
			{
				shared_data->camera.position.z += 0.1;
			}
			else if(key == 'd')
			{
				shared_data->camera.position.z -= 0.1;
			}
			/*
			else if(key == SDLK_1)
			{
				render_settings_set_flag(shared_data->settings, flag_display_wireframe_only, false);

				bool bValue = !render_settings_check_flag(shared_data->settings, flag_display_wireframe_entirely);
				render_settings_set_flag(shared_data->settings, flag_display_wireframe_entirely, bValue);
			}
			else if(key == SDLK_2)
			{

				render_settings_set_flag(shared_data->settings, flag_display_wireframe_entirely, false);

				bool bValue = !render_settings_check_flag(shared_data->settings, flag_display_wireframe_only);
				render_settings_set_flag(shared_data->settings, flag_display_wireframe_only, bValue);


			}
			else if(key == SDLK_3)
			{
				bool bValue = !render_settings_check_flag(shared_data->settings, flag_display_filled_triangles);
				render_settings_set_flag(shared_data->settings, flag_display_filled_triangles, bValue);
			}
			else if(key == SDLK_4)
			{
				bool bValue1 = !render_settings_check_flag(shared_data->settings, flag_display_filled_triangles);
				bool bValue2 = !render_settings_check_flag(shared_data->settings, flag_display_wireframe_only);
				render_settings_set_flag(shared_data->settings, flag_display_filled_triangles, bValue1);
				render_settings_set_flag(shared_data->settings, flag_display_wireframe_only, bValue2);

			}
			else if(key == SDLK_c)
			{
				bool bValue = !render_settings_check_flag(shared_data->settings, flag_back_face_culling);
				render_settings_set_flag(shared_data->settings, flag_back_face_culling, bValue);
			}*/
			
		}
		break;
		default:
		break;
	}
}

APP_END(AppEnd)
{
	
}