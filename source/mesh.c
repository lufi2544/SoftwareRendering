


////// MESH //////

// TODO pass the entity position here.
internal_f bool
can_render_face(vec4_t _face_verteces[3], vec3_t _camera_position)
{
	bool result = false;
		
	// 1. Get the face normal.
	vec3_t a = vec3_from_vec4(_face_verteces[0]);
	vec3_t b = vec3_from_vec4(_face_verteces[1]);
	vec3_t c = vec3_from_vec4(_face_verteces[2]);
	
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


internal_f s8
compare_triangle(const void *_a, const void *_b)
{
	triangle_t *triangle_a  = (triangle_t*)_a;
	triangle_t *triangle_b  = (triangle_t*)_b;
	
	if(triangle_a->avg_depth < triangle_b->avg_depth)
	{
		return -1;
	}
	else if(triangle_a->avg_depth == triangle_b->avg_depth)
	{
		return 0;
	}
	
	// a_avg >= b_avg
	return 1;
}

internal_f void 
mesh_render(mesh_t *_mesh)
{				
	SCRATCH();
		
	vec3_t camera_position = { 0, 0, 0 };
	g_camera.position = camera_position;	

    
    f32 fov_coefficient = 2000;// set this to 2000 and fix bug.
	
    _mesh->rotation.y += 0.01;
	_mesh->rotation.x += 0.01;
	
	//TODO: (juanes.rayo): adding this to the an entity value, so we render the entity and take the position
	//vec3_t position = _mesh->location;
	
	
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
		
		// 1.Check the face verteces and apply transformation
		vec4_t transformed_verteces[3];
		mat4_t scale_matrix = mat4_make_scale(_mesh->scale.x, _mesh->scale.y, _mesh->scale.z); 
		mat4_t translation_matrix = mat4_make_translation(_mesh->translation.x, _mesh->translation.y, _mesh->translation.z);
		
		for(s32 j = 0; j < 3; ++j)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_verteces[j]);
			transformed_vertex = mat4_mul_vec4(scale_matrix, transformed_vertex);
			transformed_vertex = mat4_mul_vec4(translation_matrix, transformed_vertex);
			
			// Use the rotation matrix for this.
			//transformed_vertex = vec4_from_vec3(vec3_rotate_x(vec3_from_vec4(transformed_vertex), _mesh->rotation.x));
			//transformed_vertex = vec4_from_vec3(vec3_rotate_y(vec3_from_vec4(transformed_vertex), _mesh->rotation.y));			
			//transformed_vertex = vec4_from_vec3(vec3_rotate_z(vec3_from_vec4(transformed_vertex), _mesh->rotation.z));
			
			// We are using a +y is down as in this engine the screen up scales that way, flipping that so the meshes can be visialuzed correctly.
			transformed_vertex.y *= -1;
			
			// push the mesh 10u to the screen, left-handed coord system
			
			
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
			vec2_t projected_point = project_vec3(vec3_from_vec4(transformed_verteces[k]), fov_coefficient);
			
			// NOTE: By adding this we would treat the triangles relative to the center of the screen
			projected_point.x += (window_width/ 2);
			projected_point.y += (window_height / 2);
			
			// saving the point for the triangle in screen space.
			projected_triangle.points[k] = projected_point;
		}
		
		projected_triangle.avg_depth = ((f32)transformed_verteces[0].z + (f32)transformed_verteces[1].z + (f32)transformed_verteces[2].z) / 3;
		projected_triangle.color = 0x00000000;
		
		LIST_ADD(temp_arena, mesh_triangles_list, projected_triangle, triangle_t);
	}
	
	
	// Sort by avg_depth -> kind of a Z buffer.
	merge_sort(&mesh_triangles_list.head, &compare_triangle);
	
	{
		triangle_t *triangle = 0;
		LIST_FOREACH(triangle_t, triangle, mesh_triangles_list)
		{		
			if(render_settings_check_flag(flag_display_filled_triangles) || render_settings_check_flag(flag_display_filled_triangles_wire))
			{
				draw_filled_triangle(triangle, triangle->color);
			}
			
			bool bDrawDots = (!render_settings_check_flag(flag_display_wireframe_only) || (render_settings_check_flag(flag_display_wireframe_entirely)));		
			draw_linear_triangle(triangle, COLOR_RED, bDrawDots);
		}
	}
			
	SCRATCH_END();
}
