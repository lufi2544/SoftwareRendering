/* date = March 7th 2025 9:38 am */

//////////////// TRIANGLE RELATED STUFF ///////////////

/** Defining face. Stores the vertex indexes.  */

typedef u32 color_t;

/** Stored the 3 points in screen space to form a triangle.  */
typedef struct 
{	
	vec2_t points[3];
	color_t color;
	f32 avg_depth;	
	texture_uv_t texture_coords[3];
	
} triangle_t;



internal_f void 
print_face(face_t _face)
{
	printf("a: %i b: %i c: %i \n", _face.a, _face.b, _face.c);
}
