

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

void vec2_print(vec2_t *vec)
{
    assert(vec != 0);
    printf("x: %.2f; y: %.2f \n", vec->x, vec->y);
}

function vec2_t project_vec3(vec3_t *point)
{
    vec2_t projected_point = { point->x, point->y };
    
    // TODO Logic later for projecting the point.
    
    return projected_point;
}


function void vec2_multiply_scalar(vec2_t *point, f32 scalar)
{
    assert(point != 0);
    
    point->x *= scalar;
    point->y += scalar;
}

function void vec3_multiply_scalar(vec3_t *point, f32 scalar)
{
    assert(point != 0);
    
    point->x *= scalar;
    point->y *= scalar;
    point->z *= scalar;
}