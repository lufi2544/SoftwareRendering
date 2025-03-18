
#include <math.h>

/////////// MATH ///////////

// Try to use the types according to the memory access limits.
typedef struct
{
    f32 x;
    f32 y;
} vec2_t;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;        
    
} vec3_t;


/////// VECTOR ///////

internal void
vec2_print(vec2_t *vec)
{
    assert(vec != 0);
    printf("x: %.2f; y: %.2f \n", vec->x, vec->y);
}

internal void 
vec3_print(vec3_t vec)
{
    printf("x: %.2f; y: %.2f z: %.2f \n", vec.x, vec.y, vec.z);
}

function vec2_t project_vec3(vec3_t *point, f32 *fov)
{
    vec2_t projected_point = { 
		(point->x * (*fov)) / point->z, 
		(point->y * (*fov)) / point->z };       
    return projected_point;
}


function void vec2_multiply_scalar(vec2_t *point, f32 scalar)
{
    assert(point != 0);
    
    point->x *= scalar;
    point->y *= scalar;
}

function void vec3_multiply_scalar(vec3_t *point, f32 scalar)
{
    assert(point != 0);
    
    point->x *= scalar;
    point->y *= scalar;
    point->z *= scalar;
}

function vec3_t vec3_rotate_x(vec3_t v, f32 angle)
{
    vec3_t rotated_vector = 
    { 
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle)
    };
    
    return rotated_vector;
}

function vec3_t vec3_rotate_y(vec3_t v, f32 angle)
{
    vec3_t rotated_vector =
    { 
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle)
    };
    
    return rotated_vector;
}

function vec3_t vec3_rotate_z(vec3_t v, f32 angle)
{
    vec3_t rotated_vector =
    { 
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };
    
    return rotated_vector;
}


