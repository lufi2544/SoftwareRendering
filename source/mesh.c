


////// MESH //////

// TODO pass the entity position here. When we have world space.
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
	vec3_t camera_to_face = vec3_subtract(a, _camera_position);
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

global_f void 
mesh_render(mesh_t *_mesh)
{				
	SCRATCH();
		
	vec3_t camera_position = { 0, 0, 0 };
	g_camera.position = camera_position;	
        	
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
		
		// 1.Check the face verteces and apply space matreces
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
			
			// The screen space grows from up to down.ks
			//transformed_vertex.y *= -1;						
			
			
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
			//OPTIMIZE:  we can obtain the avg_depth here.
			
			vec4_t projected_point = mat4_mul_vec4_project(projection_matrix, transformed_verteces[k]);
			
			// saving the point for the triangle in screen space.
			vec2_t point = {};
			point.x = projected_point.x;
			point.y = projected_point.y;
											
			// Scale into the view, since we are in a -1 to +1 space
			point.x *= g_window_width ;
			point.y *= g_window_height;
			projected_triangle.points[k] = point;						
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
