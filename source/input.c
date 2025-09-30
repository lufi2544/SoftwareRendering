
////////////////// INPUT ////////////////


// TODO Make an input system for the application layer
internal_f void process_input()
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
