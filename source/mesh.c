


////// MESH //////

// TODO pass the entity position here.
internal bool
can_render_face(vec3_t _mesh_position, vec3_t _face_verteces[3], vec3_t _camera_position)
{
	bool result = false;
	
	// 1. Get the face normal.
	vec3_t a = _face_verteces[0];
	vec3_t b = _face_verteces[1];
	vec3_t c = _face_verteces[2];
	
	vec3_t ab = vec3_subtract(a, b);
	vec3_t ac = vec3_subtract(a, c);
	
	vec3_t normal = vec3_cross(ab, ac);	
	
	// 2. See if they are pointing to different directions.	
	result = vec3_dot(_camera_position, normal) > 0;
	
	return result;
}

internal void 
mesh_render(mesh_t *_mesh)
{				
	TEMP_MEMORY();
		
	vec3_t camera_position = { 0, 0, -5 };
	g_camera.position = camera_position;
    
    f32 fov_coefficient = 1500;// set this to 2000 and fix bug.
    _mesh->rotation.y += 0.01;
	
	//TODO: (juanes.rayo): adding this to the an entity value, so we render the entity and take the position
	vec2_t position = { 800, 800 };
	
	
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
		
		vec3_t entity_position = { 800, 800,0 };
		/*if(!can_render_face(entity_position, face_verteces, camera_position))
		{
			continue;
		}*/
		
		triangle_t projected_triangle;		
		
		if(i == 965)
		{
			int a = 0;
		}
		// We transform those points in 3D to 2D screen space
		for(s32 j = 0; j < 3; ++j)
		{
			vec3_t transformed_vertex = face_verteces[j];					
			transformed_vertex = vec3_rotate_x(transformed_vertex, _mesh->rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, _mesh->rotation.y);
			transformed_vertex.y *= -1;
			
			// We are using a +y is down as in this engine the screen up scales that way, flipping that so the meshes can be visialuzed correctly.
			transformed_vertex = vec3_rotate_z(transformed_vertex, _mesh->rotation.z);
			
			transformed_vertex.z -= g_camera.position.z;
			vec2_t projected_point = project_vec3(transformed_vertex, fov_coefficient);
			
			// saving the point for the triangle in screen space.
			projected_triangle.points[j] = projected_point;				
		}			
		
		list_node_t* node = LIST_ADD(temp_arena, mesh_triangles_list, projected_triangle, triangle_t);
		triangle_t t = *((triangle_t*)node->data);		
	}
		
	list_node_t *it = mesh_triangles_list.head;
	while(it != 0)
	{
		triangle_t triangles = *((triangle_t*)it->data);
		
		vec2_t position_0 = {triangles.points[0].x + position.x, triangles.points[0].y + position.y};
		draw_rect(position_0.x, position_0.y, 5, 5, 0x000000);			
		
		vec2_t position_1 ={triangles.points[1].x + position.x, triangles.points[1].y + position.y};
		draw_rect(position_1.x, position_1.y, 5, 5, 0x000000);				
		
		vec2_t position_2 ={triangles.points[2].x + position.x, triangles.points[2].y + position.y};
		draw_rect(position_2.x, position_2.y, 5, 5, 0x000000);						
		
		// A-B
	    draw_line(position_0.x, position_0.y, position_1.x, position_1.y);
		
		// B-C
	    draw_line(position_1.x, position_1.y, position_2.x, position_2.y);
		
		// C-A
	    draw_line(position_2.x, position_2.y, position_0.x, position_0.y);
		it = it->next_sibling;
	}
			
	TEMP_MEMORY_END();
}
