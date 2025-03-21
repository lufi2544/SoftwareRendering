
#include <math.h>

/////////// MATH ///////////



#define PI 3.14159

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

///// V3 //////
internal void 
vec3_print(vec3_t vec)
{
    printf("x: %.2f; y: %.2f z: %.2f \n", vec.x, vec.y, vec.z);
}

internal vec3_t
vec3_add(vec3_t v, vec3_t other)
{
	vec3_t result;
	result.x = v.x + other.x;
	result.y = v.y + other.y;
	result.z = v.z + other.z;
	
	return result;
}

internal vec2_t
project_vec3(vec3_t point, f32 fov)
{
	
    vec2_t projected_point = { 
		(point.x * (fov)) / point.z, 
		(point.y * (fov)) / point.z };       
	
    return projected_point;
}

internal void
vec3_multiply_scalar(vec3_t *point, f32 scalar)
{    
    point->x *= scalar;
    point->y *= scalar;
    point->z *= scalar;
}

internal void
vec3_divide_scalar(vec3_t *v, f32 s)
{
	v->x /= s;
	v->y /= s;
	v->z /= s;
}

internal vec3_t
vec3_rotate_x(vec3_t v, f32 angle)
{
    vec3_t rotated_vector = 
    { 
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle)
    };
    
    return rotated_vector;
}

internal vec3_t
vec3_rotate_y(vec3_t v, f32 angle)
{
    vec3_t rotated_vector =
    { 
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle)
    };
    
    return rotated_vector;
}

internal vec3_t
vec3_rotate_z(vec3_t v, f32 angle)
{
    vec3_t rotated_vector =
    { 
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };
    
    return rotated_vector;
}

internal f32 
vec3_length(vec3_t v)
{
	f32 result;	
	result = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));	
	return result;
}

internal vec3_t
vec3_subtract(vec3_t a, vec3_t b)
{
	vec3_t result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;	
	result.z = a.z - b.z;
	return result;
}

internal vec3_t
vec3_cross(vec3_t a, vec3_t b)
{
	vec3_t result = 
	{
		result.x = (a.y * b.z) - (a.z * b.y),		
		result.y = (a.z * b.x) - (a.x * b.z),	
		result.z = (a.x * b.y) - (a.y * b.x)		
	};
	
	return result;
}

internal f32 
vec3_dot(vec3_t a, vec3_t b)
{
	f32 result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);	
	return result;
}

internal vec3_t 
vec3_normalize(vec3_t v)
{
	vec3_t result;
	
	f32 lenght = vec3_length(v);
	
	result.x = v.x / lenght;
	result.y = v.y / lenght;
	result.z = v.z / lenght;
	
	
	return result;
}


////// VEC2 //////

internal void
vec2_print(vec2_t *vec)
{
    assert(vec != 0);
    printf("x: %.2f; y: %.2f \n", vec->x, vec->y);
}

internal f32 
vec2_length(vec2_t v)
{
	f32 result;	
	result = sqrt((v.x * v.x) + (v.y * v.y));	
	return result;
}

internal void 
vec2_multiply_scalar(vec2_t *point, f32 scalar)
{
    assert(point != 0);
    
    point->x *= scalar;
    point->y *= scalar;
}

internal vec2_t
vec2_subtract(vec2_t a, vec2_t b)
{
	vec2_t result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;	
	return result;
}


internal void
vec2_divide_scalar(vec2_t *v, f32 s)
{
	v->x /= s;
	v->y /= s;
}
