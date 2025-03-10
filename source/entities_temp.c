


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
function void 
fill_cube_points()
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
internal void 
temp_cube_create()
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

internal void 
temp_cube_render()
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

internal void 
cube_mesh_init(mesh_t *cube, memory_arena_t *_arena)
{
	if(cube == 0)
	{
		return;
	}
		
	// Init the cube.
	
	// Verteces
	cube->verteces = PushArray(_arena, N_CUBE_VERTECES, vec3_t);
	
	//Faces
	cube->faces = PushArray(_arena, N_CUBE_FACES, face_t);
	
	//Triangles, will be set at runtime when recalculated every frame.
	cube->triangles = PushArray(_arena, N_CUBE_FACES, triangle_t);
		
	for(s32 v = 0; v < N_CUBE_VERTECES; ++v)
	{
		cube->verteces[v] = g_cube_verteces[v];
	}
	
	for(s32 f = 0; f < N_CUBE_FACES; ++f)
	{
		cube->faces[f] = g_cube_faces[f];
	}	
}

internal void
cube_mesh_update(mesh_t *cube)
{		
	
	// TODO figure out a better method here.

	vec3_t camera_position = { 0, 0, -5 };
	g_camera.position = camera_position;
    
    f32 fov_coefficient = 1000;
    cube->rotation.y += 0.01;
    cube->rotation.z += 0.01;
    cube->rotation.x += 0.01;
	
	// Maybe storing the faces num is a good idea for future meshes of course
	for(s32 i = 0; i < N_CUBE_FACES; ++i)
	{
		face_t cube_face = cube->faces[i];		
		
		// We get the points in the space from the faces array.
		vec3_t face_verteces[3];
		face_verteces[0] = cube->verteces[cube_face.a];
		face_verteces[1] = cube->verteces[cube_face.b];
		face_verteces[2] = cube->verteces[cube_face.c];
		
		triangle_t projected_triangle;
		
		// We transform those points in 3D to 2D screen space
		for(s32 j = 0; j < 3; ++j)
		{
			vec3_t transformed_vertex = face_verteces[j];					
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube->rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube->rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube->rotation.z);
			
			transformed_vertex.z -= g_camera.position.z;
			vec2_t projected_point = project_vec3(&transformed_vertex, &fov_coefficient);
			
			// saving the point for the triangle in screen space.
			projected_triangle.points[j] = projected_point;
			
		}			
		
		cube->triangles[i] = projected_triangle;
	}
	
}


internal void 
cube_mesh_render(mesh_t *cube)
{		
	vec2_t position = { 1000, 1000 };
	for(s32 i = 0; i < N_CUBE_FACES; ++i)
	{
		triangle_t triangles = cube->triangles[i];
		
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
	}
}