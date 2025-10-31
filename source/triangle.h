/* date = March 7th 2025 9:38 am */

//////////////// TRIANGLE RELATED STUFF ///////////////


/** Stored the 3 points in screen space to form a triangle.  */
typedef struct 
{	
	vec4_t points[3];
	texture_uv_t texture_coords[3];
	color_t color;
	f32 avg_depth;	
	
} triangle_t;


internal_f void 
print_face(face_t _face)
{
	printf("a: %i b: %i c: %i \n", _face.a, _face.b, _face.c);
}
