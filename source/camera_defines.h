/* date = February 21st 2025 6:01 pm */

typedef struct 
{
    vec3_t position;
    vec3_t rotation;
    f32 fov_angle;
} camera_t;

global camera_t g_camera;
global bool g_camera_moving = true;