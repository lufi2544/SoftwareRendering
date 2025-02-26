

////////////// RENDERER //////////////


#define GRID_DEFAULT_COLOR 0xA0A0A0

global camera_t camera;

global vec3_t cube_rotation;

void render(void)
{
    
    vec3_t camera_position = { 0, 0, -5 };
    camera.position = camera_position;
    
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
        
        transformed_point.z -= camera.position.z;
        vec2_t projected_point = project_vec3(&transformed_point, &fov_coefficient);
        
        cube_projected_points[i] = projected_point;
    }
    
    SDL_RenderClear(renderer);
    
    map_texture_to_pixels_buffer();
    
    // We have mapped the pixels on the screen on the Texture, changing the pixels on the screen will make the texture to change.
    clear_color_buffer(0xFFFFFF);
    
       
    f32 x = 1000.0f;
    f32 y = 800.0f;
    for(u32 i = 0; i < POINTS_NUM; ++i)
    {
        vec2_t point = cube_projected_points[i];        
        point.x += x;
        point.y += y;
        draw_rect(point.x, point.y,  10, 10, 0x00000000);
    }
    
    vec2_t Position = { 1000, 1000 };  
    //draw_pixel(Position, (0x00000000));
    
    SDL_RenderPresent(renderer);
}


