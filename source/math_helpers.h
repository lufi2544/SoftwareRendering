
#include <math.h>

/////////// MATH ///////////



#define PI 3.14159

typedef struct
{
	f32 m[4][4];
	
} mat4_t;


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


typedef struct
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} vec4_t;


/////// VECTOR ///////

///// MATRIX //////

global_f mat4_t 
mat4_identity(void)
{
	
	mat4_t result =
	{ .m = {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		}};
	
	return result;	
}



global_f mat4_t
mat4_mul_mat4(mat4_t a, mat4_t b)
{
	mat4_t result;
	
	for(u8 i = 0; i < 4; ++i)
	{
		for(u8 j = 0; j < 4; ++j)
		{
			result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
		}
	}	
	
	return result;
}

global_f mat4_t
mat4_make_scale(f32 sx, f32 sy, f32 sz)
{	        	
	mat4_t result = 
	{{
			{sx, 0, 0, 0},
			{0, sy, 0, 0},
			{0, 0, sz, 0},
			{0, 0, 0, 1}
		}};		
	
	return result;
}

global_f mat4_t
mat4_make_translation_matrix(f32 tx, f32 ty, f32 tz)
{
	mat4_t result = 
	{
		{
			{1, 0, 0, tx},
			{0, 1, 0, ty},
			{0, 0, 1, tz},
			{0, 0, 0, 1}
		}
	};
					
	return result;
}

global_f mat4_t 
mat4_make_rotation_matrix_x(f32 angle)
{
	mat4_t result = 
	{
		{
			{1, 0, 0, 0},
			{0, cos(angle), -sin(angle), 0},
			{0, sin(angle), cos(angle), 0},
			{0, 0, 0, 1}
		}
	};
	
	return result;
}

global_f mat4_t 
mat4_make_rotation_matrix_y(f32 angle)
{
	// keep the direction to rotate it counter clock wise.
	mat4_t result = 
	{
		{
			{cos(angle), 0, sin(angle), 0},
			{0, 1, 0, 0},
			{-sin(angle), 0, cos(angle), 0},
			{0, 0, 0, 1}
		}
	};
	
	return result;
}

global_f mat4_t 
mat4_make_rotation_matrix_z(f32 angle)
{
	mat4_t result = 
	{
		{
			{cos(angle), -sin(angle), 0, 0},
			{sin(angle), cos(angle), 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		}
	};
	
	return result;
}

global_f mat4_t
mat4_make_perspective(f32 _fov, f32 _aspect, f32 _znear, f32 _zfar)
{
	mat4_t m = {{{ 0 }}};
	m.m[0][0] = _aspect * (1 / tan(_fov / 2)); 
	m.m[1][1] = 1 / tan(_fov / 2); // flipping here the y coord as in screen space we are growing downwards
	m.m[2][2] = _zfar / (_zfar - _znear);
	m.m[3][3] = (-_zfar * _znear) / (_zfar - _znear);
	
	// Storing the original Z value in the W value of the Matrix.
	// Saving the original value becaue this is going to be saved and used in texturing, etc.
	m.m[3][2] = 1.0f;		
		
	return m;
}

global_f vec4_t
mat4_mul_vec4(mat4_t m, vec4_t v)
{
	vec4_t result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
	
	return result;
}


global_f vec4_t
mat4_mul_vec4_project(mat4_t _mat_proj, vec4_t _v)
{
	vec4_t result = mat4_mul_vec4(_mat_proj, _v);
	
	// perform the perspective divide with the original Z-value
	if(result.w != 0.0f)
	{
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	
	// this is now in a kind of image space per say, from -1 to +1.
	return result;
}

global_f vec4_t
rotate_vec4_x(vec4_t v, f32 angle)
{
	
	mat4_t result = 
	{
		{
			{1, 0, 0, 0 },
			{0, cos(angle), -sin(angle), 0 },
			{0, sin(angle), cos(angle), 0 },
			{0, 0, 0, 1 }
		}
	};
	
	return mat4_mul_vec4(result, v);		
}

///// V3 //////
global_f void 
vec3_print(vec3_t vec)
{
    printf("x: %.2f; y: %.2f z: %.2f \n", vec.x, vec.y, vec.z);
}


global_f vec3_t 
vec3_from_vec4(vec4_t v)
{
	vec3_t result = { v.x, v.y, v.z };		
	return result;
}


global_f vec4_t
vec4_from_vec3(vec3_t v)
{
	vec4_t result = {v.x, v.y, v.z, 1};	
	return result;
}

global_f vec3_t
vec3_add(vec3_t v, vec3_t other)
{
	vec3_t result;
	result.x = v.x + other.x;
	result.y = v.y + other.y;
	result.z = v.z + other.z;
	
	return result;
}

global_f vec2_t
project_vec3(vec3_t point, f32 fov)
{
	
    vec2_t projected_point = { 
		(point.x * (fov)) / point.z, 
		(point.y * (fov)) / point.z };       
	
    return projected_point;
}

internal_f void
vec3_multiply_scalar(vec3_t *point, f32 scalar)
{    
    point->x *= scalar;
    point->y *= scalar;
    point->z *= scalar;
}

internal_f void
vec3_divide_scalar(vec3_t *v, f32 s)
{
	v->x /= s;
	v->y /= s;
	v->z /= s;
}

internal_f vec3_t
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


internal_f vec3_t
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

internal_f vec3_t
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

internal_f f32 
vec3_length(vec3_t v)
{
	f32 result;	
	result = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));	
	return result;
}

internal_f vec3_t
vec3_subtract(vec3_t a, vec3_t b)
{
	vec3_t result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;	
	result.z = a.z - b.z;
	return result;
}

internal_f vec3_t
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

internal_f f32 
vec3_dot(vec3_t a, vec3_t b)
{
	f32 result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);	
	return result;
}

internal_f vec3_t 
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

internal_f void
vec2_print(vec2_t *vec)
{
    assert(vec != 0);
    printf("x: %.2f; y: %.2f \n", vec->x, vec->y);
}

internal_f f32 
vec2_length(vec2_t v)
{
	f32 result;	
	result = sqrt((v.x * v.x) + (v.y * v.y));	
	return result;
}

internal_f void 
vec2_multiply_scalar(vec2_t *point, f32 scalar)
{
    assert(point != 0);
    
    point->x *= scalar;
    point->y *= scalar;
}


// We subtract a - b
internal_f vec2_t
vec2_subtract(vec2_t a, vec2_t b)
{
	vec2_t result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;	
	return result;
}


internal_f void
vec2_divide_scalar(vec2_t *v, f32 s)
{
	v->x /= s;
	v->y /= s;
}


internal_f f32
vec2_cross(vec2_t u, vec2_t v)
{
	return u.x * v.y - u.y * v.x;
}


