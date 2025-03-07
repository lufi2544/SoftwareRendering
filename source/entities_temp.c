


///// TEMP CUBE MESH//////
global vec3_t cube_rotations[2];


/////////// Squared Points Base Cube //////////
#define POINTS_PER_ROW 9
#define POINTS_NUM_SQUARED POINTS_PER_ROW * POINTS_PER_ROW
#define POINTS_NUM POINTS_NUM_SQUARED * POINTS_PER_ROW

global vec3_t cube_points[POINTS_NUM];
global vec2_t cube_projected_points[POINTS_NUM];


// Fixed points per line cube @see POINTS_PER_ROW
// Normalized values of the cube points in space.
function void fill_cube_points()
{    
    assert(ArrayCount(cube_points) == POINTS_NUM);
    const f32 coefficient = (f32)2 / (f32)(POINTS_PER_ROW - 1);
    //printf("cube: Targeted Points: %i; Points per Row: %i;  Coefficient: %.4f \n", POINTS_NUM, POINTS_PER_ROW, coefficient);
    f32 y_coeff = -1;
    for(u32 y_index = 0; y_index < POINTS_PER_ROW; ++y_index, y_coeff += coefficient)
    {
        f32 x_coeff = -1;
        for(u32 x_index = 0; x_index < POINTS_PER_ROW; ++x_index, x_coeff += coefficient)
        {
            f32 z_coeff = -1;
            for(u32 z_index = 0; z_index < POINTS_PER_ROW; ++z_index, z_coeff += coefficient)
            {
                //printf("x: %i, y: %i, z: %i \n", x_index, y_index, z_index);
                //printf("Filling point x: %.2f y: %.2f z: %.2f \n", x_coeff, y_coeff, z_coeff);
                vec3_t point = { x_coeff, y_coeff, z_coeff  };
                cube_points[(y_index * POINTS_NUM_SQUARED) + (x_index * POINTS_PER_ROW) + z_index] = point;
            }
            z_coeff = -1;
        }
        x_coeff = -1;
    }
    
    
    printf("Filling cube points \n");
}


/////////// Cube created with different square as a delimitation points to be rendered and visible.
internal void temp_cube_create()
{
    vec3_t camera_position = { 0, 0, -5 };
    g_camera.position = camera_position;
    
    f32 fov_coefficient = 1000;
    cube_rotations[0].y += 0.01;
    cube_rotations[0].z += 0.01;
    cube_rotations[0].x += 0.01;
    	
	
    for(u32 i = 0; i < POINTS_NUM; ++i)
    {
        vec3_t point = cube_points[i];
		
        vec3_t transformed_point = vec3_rotate_y(point, cube_rotations[0].y);
        transformed_point = vec3_rotate_x(transformed_point, cube_rotations[0].x);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotations[0].z);
        
        transformed_point.z -= g_camera.position.z;
        vec2_t projected_point = project_vec3(&transformed_point, &fov_coefficient);
        
        cube_projected_points[i] = projected_point;
    }   

}

internal void temp_cube_render()
{
    f32 x = 1000.0f;
    f32 y = 800.0f;
    for(u32 i = 0; i < POINTS_NUM; ++i)
    {
        vec2_t point = cube_projected_points[i];        
        point.x += x;
        point.y += y;
        draw_rect(point.x, point.y,  10, 10, 0x00000000);
    }
}    


////////////////// Cube Mesh //////////////////



#define N_CUBE_VERTECES 8
global const vec3_t g_cube_verteces[N_CUBE_VERTECES] = 
{
	{ -1, -1, -1 },
	{ -1, 1, -1 },
	{ 1, 1, -1 },
	{ 1, -1, -1 },
	{ 1, 1, 1 },
	{ 1, -1, 1 },
	{ -1, 1, 1 },
	{ -1, -1, 1 }
};


// Basically we are pointing to the cube_verteces array on top.
#define N_CUBE_FACES (6 * 2) // In this case we have to count with the fron face and the back face of the meshes.

// 2 Triangles for each face.
global triangle_t g_cube_triangles[N_CUBE_FACES];

global const face_t g_cube_faces[N_CUBE_FACES] = 
{
	//front
	{ 0, 1, 2},
	{ 0, 2, 3 },
	
	// right
	{ 3, 2, 4 },
	{ 3, 4, 5 },
	
	//back
	{ 5, 4, 6 },
	{ 5, 6, 7 },
	
	//left
	{ 7, 6, 1 },
	{ 7, 1, 0 },
	
	//top
	{ 1, 6, 4 },
	{ 1, 4, 2 },
	
	//bottom
	{ 5, 7, 0 },
	{ 5, 0, 3 },	
};


internal void cube_mesh_create()
{		
	vec3_t camera_position = { 0,0, -5 };
	g_camera.position = camera_position;
    
    f32 fov_coefficient = 1000;
    cube_rotations[1].y += 0.01;
    cube_rotations[1].z += 0.01;
    cube_rotations[1].x += 0.01;
		
	for(s32 i = 0; i < N_CUBE_FACES; ++i)
	{
		face_t cube_face = g_cube_faces[i];		
		
		// We get the points in the space from the faces array.
		vec3_t face_verteces[3];
		face_verteces[0] = g_cube_verteces[cube_face.a];
		face_verteces[1] = g_cube_verteces[cube_face.b];
		face_verteces[2] = g_cube_verteces[cube_face.c];
		
		triangle_t projected_triangle;
		
		// We transform those points in 3D to 2D screen space
		for(s32 j = 0; j < 3; ++j)
		{
			vec3_t transformed_vertex = face_verteces[j];
			
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotations[1].x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotations[1].y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotations[1].z);
			
			transformed_vertex.z -= g_camera.position.z;
			vec2_t projected_point = project_vec3(&transformed_vertex, &fov_coefficient);
			
			// saving the point for the triangle in screen space.
			projected_triangle.points[j] = projected_point;
			
		}			
		
		g_cube_triangles[i] = projected_triangle;
	}
	
}


internal void cube_mesh_render()
{
	for(s32 i = 0; i < N_CUBE_FACES; ++i)
	{
		triangle_t triangle = g_cube_triangles[i];
		draw_rect(triangle.points[0].x + 1000, triangle.points[0].y + 800, 5, 5, 0x000000);
		draw_rect(triangle.points[1].x + 1000, triangle.points[1].y + 800, 5, 5, 0x000000);
		draw_rect(triangle.points[2].x + 1000, triangle.points[2].y + 800, 5, 5, 0x000000);						
	}
}



