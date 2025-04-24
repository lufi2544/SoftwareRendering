


////// MESH //////

// TODO pass the entity position here.
internal bool
can_render_face(vec3_t _face_verteces[3], vec3_t _camera_position)
{
	bool result = false;
		
	// 1. Get the face normal.
	vec3_t a = _face_verteces[0];
	vec3_t b = _face_verteces[1];
	vec3_t c = _face_verteces[2];
	
	vec3_t ab = vec3_subtract(b, a);
	vec3_t ac = vec3_subtract(c, a);
	
	vec3_t normal = vec3_cross(ab, ac);
	
	// 2. See if they are pointing to different directions.		
	vec3_t camera_to_face = vec3_subtract(_camera_position, a);
	vec3_t camera_to_face_n = vec3_normalize(camera_to_face);
	f32 dot = vec3_dot(normal, camera_to_face_n);
	
	result = dot > 0;
	
	return result;
}

internal void 
mesh_render(mesh_t *_mesh)
{				
	TEMP_MEMORY();
		
	vec3_t camera_position = { 0, 0, 0 };
	g_camera.position = camera_position;	

    
    f32 fov_coefficient = 2000;// set this to 2000 and fix bug.
	
    _mesh->rotation.y += 0.01;
	_mesh->rotation.x += 0.01;
	
	//TODO: (juanes.rayo): adding this to the an entity value, so we render the entity and take the position
	vec3_t position = { window_width /2, window_height /2, 0 };
	
	
	list_t mesh_triangles_list = LIST(temp_arena);
	
	for(u32 i = 0; i < _mesh->face_num; ++i)
	{
		face_t mesh_face = _mesh->faces[i];
		
		// FACE CULLING 
		//if the camera rotation is different than the face normal, then we skip the rendering for this face, as they are pointing to different orientations.
		
		
		vec3_t face_verteces[3];
		face_verteces[0] = _mesh->verteces[mesh_face.a];
		face_verteces[1] = _mesh->verteces[mesh_face.b];
		face_verteces[2] = _mesh->verteces[mesh_face.c];		
		
		vec3_t entity_position = { 0,0, 1000 };
		
		
		vec3_t transformed_verteces[3];
		// 1.Check the face verteces and apply transformation
		for(s32 j = 0; j < 3; ++j)
		{
			vec3_t transformed_vertex = face_verteces[j];					
			transformed_vertex = vec3_rotate_x(transformed_vertex, _mesh->rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, _mesh->rotation.y);
			transformed_vertex.y *= -1;
			// We are using a +y is down as in this engine the screen up scales that way, flipping that so the meshes can be visialuzed correctly.
			transformed_vertex = vec3_rotate_z(transformed_vertex, _mesh->rotation.z);
			
			// push the mesh 10u to the screen, left-handed coord system
			transformed_vertex.z += 10;
			
			// Transform to world position
			//transformed_vertex = vec3_add(transformed_vertex, position);
			
			transformed_verteces[j] = transformed_vertex;									
		}
		
		// 3. Check the Face Culling from the camera
		if(!can_render_face(transformed_verteces, camera_position) && render_settings_check_flag(flag_back_face_culling))
		{
			continue;
		}	
		
		
		// 2. Project the verteces to screen space and create a triangle
		triangle_t projected_triangle;		
		for(u32 k = 0; k < 3; ++k)			
		{						
			vec2_t projected_point = project_vec3(transformed_verteces[k], fov_coefficient);
			projected_point.x += position.x;
			projected_point.y += position.y;
			
			// saving the point for the triangle in screen space.
			projected_triangle.points[k] = projected_point;															
		}		
	
		
		LIST_ADD(temp_arena, mesh_triangles_list, projected_triangle, triangle_t);
	}
		
	list_node_t *it = mesh_triangles_list.head;
	while(it != 0)
	{
		triangle_t *triangle = LIST_NODE_DATA(it, triangle_t);
		if(render_settings_check_flag(flag_display_filled_triangles) || render_settings_check_flag(flag_display_filled_triangles_wire))
		{
			draw_filled_triangle(triangle, 0x00000000);
		}
		
		bool bDrawDots = (!render_settings_check_flag(flag_display_wireframe_only) || (render_settings_check_flag(flag_display_wireframe_entirely)));		
	    draw_linear_triangle(triangle, COLOR_RED, bDrawDots);
		it = it->next_sibling;
	}
			
	TEMP_MEMORY_END();
}
