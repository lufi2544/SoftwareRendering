/* date = March 10th 2025 9:05 am */
// Struct defining what a mesh is, basically a set of faces, verteces map, and a rotation.
typedef struct
{
	vec3_t *verteces;
	face_t *faces;
	//triangle_t *triangles;
	vec3_t rotation;
	u32 vertex_num;
	u32 face_num;
	
} mesh_t;


// Global meshes for the application.
global mesh_t *g_meshes;



