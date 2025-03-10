/* date = March 10th 2025 9:05 am */
// Struct defining what a mesh is, basically a set of faces, verteces map, and a rotation.
typedef struct
{
	vec3_t *verteces;
	face_t *faces;
	vec3_t rotation;
} mesh_t;


// Global meshes for the application.
internal mesh_t *g_meshes;
