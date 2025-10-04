////// MESH //////



// TODO pass the entity position here. When we have world space.
internal_f bool
can_render_face(vec3_t _a, vec3_t _face_normal, vec3_t _camera_position)
{	
	
	bool result = false;			
	//////
	/// See if they are pointing to different directions.		
	
	// In this case using the a vertex of the face as the point for calculating the facing vector
	
	vec3_t camera_to_face = vec3_subtract(_a, _camera_position);
	vec3_t camera_to_face_n = vec3_normalize(camera_to_face);
	f32 dot = vec3_dot(_face_normal, camera_to_face_n);
	
	result = dot < 0;
	
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



void
mesh_render(memory_t *engine_memory, mesh_t *_mesh, camera_t *_camera, mat4_t *_projection_matrix, engine_shared_data_t *_shared_data)
{				
	S_SCRATCH(engine_memory);
	
	vec3_t camera_position = _camera->position;
	
	vec3_t camera_target = {camera_position.x, camera_position.y, camera_position.z + 1.0f};
	vec3_t camera_up = {0, 1, 0};
	mat4_t view_matrix = mat4_make_view_matrix(camera_position, camera_target, camera_up);
	
	//TODO: (juanes.rayo): adding this to the an entity value, so we render the entity and take the position
	//vec3_t position = _mesh->location;
	
	
	list_t mesh_triangles_list = LIST(temp_arena);
	for(u32 i = 0; i < _mesh->face_num; ++i)
	{
		face_t mesh_face = _mesh->faces[i];
		
		// FACE CULLING 
		//if the camera rotation is different than the face normal, then we skip the rendering for this face, as they are pointing to different orientations.
		
		
		// Relating the vertex number of the mesh, numbered with the face, which contains the vertex number for the face, like (3, 4, 7)
		// which are the vertex numbers.
		vec3_t face_verteces[3];
		face_verteces[0] = _mesh->verteces[mesh_face.a];
		face_verteces[1] = _mesh->verteces[mesh_face.b];
		face_verteces[2] = _mesh->verteces[mesh_face.c];
		
		// Check the face verteces and apply space matreces
		vec4_t transformed_verteces[3];
		mat4_t scale_matrix = mat4_make_scale(_mesh->scale.x, _mesh->scale.y, _mesh->scale.z); 
		mat4_t translation_matrix = mat4_make_translation_matrix(_mesh->translation.x, _mesh->translation.y, _mesh->translation.z);
		
		mat4_t rotation_matrix_x = mat4_make_rotation_matrix_x(_mesh->rotation.x);
		mat4_t rotation_matrix_y = mat4_make_rotation_matrix_y(_mesh->rotation.y);
		mat4_t rotation_matrix_z = mat4_make_rotation_matrix_z(_mesh->rotation.z);
		
		for(s32 j = 0; j < 3; ++j)
		{
			// Apply the scale, rotation and translation matrices here for the mesh
			vec4_t transformed_vertex = vec4_from_vec3(face_verteces[j]);
			
			// World matrix
			mat4_t world_matrix = mat4_identity();
			
			//Scale Matrix
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			
			// Rotation Matrix
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			
			// Translation Matrix( this is in local space for the mesh and we are rotating also in local space, so the model would be
			// rotating around the 0,0,0)
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);
			
			
			// Translation Matrix
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);
			transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);
			
			
			// TODO: I think we can do the face culling here, since we are doing it later on and I think there is no advantage.
			
			transformed_verteces[j] = transformed_vertex;
		}
		
		
		// Calculating the normal for the face
		vec3_t a = vec3_from_vec4(transformed_verteces[0]);
		vec3_t b = vec3_from_vec4(transformed_verteces[1]);
		vec3_t c = vec3_from_vec4(transformed_verteces[2]);
		
		
		// build normal with opposite winding
		vec3_t ab = vec3_subtract(b, a);
		vec3_t ac = vec3_subtract(c, a);
		vec3_t normal = vec3_normalize(vec3_cross(ac, ab));
		
		// in left-handed view space, camera looks down +Z
		if (vec3_dot(normal, (vec3_t){0,0,1}) >= 0) {
			continue; // backface cull
		}
		
		
		
		// Project the verteces to screen space and create a triangle
		triangle_t projected_triangle;		
		for(u32 k = 0; k < 3; ++k)			
		{						
			//OPTIMIZE:  we can obtain the avg_depth here.			
			
			vec4_t projected_triangle_point = mat4_mul_vec4_project(*_projection_matrix, transformed_verteces[k]);
			
			// WHAT?
			if(projected_triangle_point.z <= 0.1)
			{
				continue;
			} 
			
			
			// The draw_filled_triangle function assumes a Y-up coordinate system,
			// where Y increases from bottom to top (i.e., (0,0) is at the bottom-left).
			// Since our screen space uses a Y-down system (Y increases from top to bottom),
			// we need to flip the Y axis during projection so triangles are correctly
			// ordered from top to bottom for rasterization.			
			//			point.y = g_window_height - point.y;
			
			
			
			vec2_t point = { projected_triangle_point.x, projected_triangle_point.y };
			point.x *= _shared_data->window_width /2;
			point.y *= _shared_data->window_height /2;
			
			point.x += _shared_data->window_width /2;
			point.y += _shared_data->window_height /2;
			
			projected_triangle.points[k] = point;			
		}
		
		projected_triangle.avg_depth = (((f32)transformed_verteces[0].z + (f32)transformed_verteces[1].z + (f32)transformed_verteces[2].z) / 3);
		
		////////
		//// FLAT LIGHT PASS
		
		projected_triangle.color = COLOR_WHITE;
		projected_triangle.color = light_flat_pass(global_light, normal, projected_triangle.color);
		
		LIST_ADD(temp_arena, mesh_triangles_list, projected_triangle, triangle_t);
	}
	
	
	// Sort by avg_depth -> kind of a Z buffer.
	merge_sort(&mesh_triangles_list.head, &compare_triangle);
	
	{
		triangle_t *triangle = 0;
		LIST_FOREACH(triangle_t, triangle, mesh_triangles_list)
		{		
			
			draw_filled_triangle(triangle, triangle->color, _shared_data);
			bool bDrawDots = (!render_settings_check_flag(_shared_data->settings, flag_display_wireframe_only) || (render_settings_check_flag(_shared_data->settings, flag_display_wireframe_entirely)));		
			//draw_linear_triangle(triangle, COLOR_RED, bDrawDots);
		}
	}
	
	SCRATCH_END();
}
