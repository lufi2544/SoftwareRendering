/* date = March 7th 2025 9:38 am */

//////////////// TRIANGLE RELATED STUFF ///////////////

/** Defining face. Stores the vertex indexes.  */
typedef struct
{
	s32 a;
	s32 b;
	s32 c;
} face_t;


/** Stored the 3 points in screen space to form a triangle.  */
typedef struct 
{	
	vec2_t points[3];
} triangle_t;
