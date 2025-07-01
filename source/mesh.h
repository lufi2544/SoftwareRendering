/* date = March 10th 2025 9:05 am */
// Struct defining what a mesh is, basically a set of faces, verteces map, and a rotation.
typedef struct
{
	vec3_t *verteces;
	face_t *faces;
	texture_uv_t *uv_coords;
	vec3_t rotation;
	vec3_t scale;
	vec3_t location;
	vec3_t translation; // TODO see if this is needed...
	u32 vertex_num;
	u32 face_num;
	u32 uv_coords_num;
	
} mesh_t;


// Global meshes for the application.
global mesh_t *g_meshes;



