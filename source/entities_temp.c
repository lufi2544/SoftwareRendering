



///// TEMP CUBE //////

#define POINTS_PER_ROW 9
#define POINTS_NUM_SQUARED POINTS_PER_ROW * POINTS_PER_ROW
#define POINTS_NUM POINTS_NUM_SQUARED * POINTS_PER_ROW

global vec3_t cube_points[POINTS_NUM];
global vec2_t cube_projected_points[POINTS_NUM];
global vec3_t cube_rotation;


// Fixed points per line cube @see POINTS_PER_ROW
// Normalized values of the cube points in space.
function void temp_cube_create()
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

// Temp cube logic
function void temp_cube_logic()
{
    vec3_t camera_position = { 0, 0, -5 };
    g_camera.position = camera_position;
    
    f32 fov_coefficient = 1000;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;
    cube_rotation.x += 0.01;
    
    // TEMP: Cube logic 
    for(u32 i = 0; i < POINTS_NUM; ++i)
    {
        vec3_t point = cube_points[i];
        vec3_t transformed_point = vec3_rotate_y(&point, cube_rotation.y);
        transformed_point = vec3_rotate_x(&transformed_point, cube_rotation.x);
        transformed_point = vec3_rotate_z(&transformed_point, cube_rotation.z);
        
        transformed_point.z -= g_camera.position.z;
        vec2_t projected_point = project_vec3(&transformed_point, &fov_coefficient);
        
        cube_projected_points[i] = projected_point;
    }   

}

function void temp_cube_render()
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