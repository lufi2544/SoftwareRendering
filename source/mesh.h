/* date = March 10th 2025 9:05 am */
// Struct defining what a mesh is, basically a set of faces, verteces map, and a rotation.

internal_f bool
can_render_face(vec3_t _a, vec3_t _face_normal, vec3_t _camera_position);

internal_f s8
compare_triangle(const void *_a, const void *_b);
