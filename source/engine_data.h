
// ---- This file contains all the data needed for the app layer of the engine


// MESH DATA

typedef struct
{
	f32 u;
	f32 v;
} texture_uv_t;

typedef struct
{
	s32 a;
	s32 b;
	s32 c;
	
	texture_uv_t a_uv;
	texture_uv_t b_uv;
	texture_uv_t c_uv;
	
} face_t;


typedef u32 color_t;

// We use the  UV coordinate method for mapping the texture to the faces.
typedef struct
{
	color_t *buffer;
	s32 w;
	s32 h;	
} texture_t;


typedef struct
{
	vec3_t *verteces;
	face_t *faces;
	texture_uv_t *uv_coords;
	texture_t *texture;
	
	u32 vertex_num;
	u32 face_num;
	u32 uv_coords_num;
	u32 texture_id;
	
	vec3_t rotation;
	vec3_t scale;
	vec3_t location;
	vec3_t translation; // TODO see if this is needed...
	
} mesh_t;


// RENDERER DATA

typedef struct
{
	u32 flags;
	f32 fov;
	f32 aspect;
	f32 znear;
	f32 zfar;
	
} render_settings;


/**
* Texture Manager that holds the memory for the different engine loaded textures.
*/

typedef struct
{	
	hash_map_t texture_map;
	
} texture_manager_t;


// CAMERA DATA

typedef struct 
{
    vec3_t position;
    vec3_t rotation;
    f32 fov_angle;
} camera_t;


typedef struct
{
	/** Memory shared among the win32 layer and the app. */
	memory_t *memory;
	
	// -- MESH --
	mesh_t *meshes;
	u32 meshes_num;
	
	
	// -- FLAGS --
	bool b_is_engine_running;
	
	
	// -- RENDER --
	/* render settings */
	render_settings *settings;
	camera_t camera;
	
	/* window stuff */
	u16 window_width;
	u16 window_height;
	
	// TODO Add the manager here 
	texture_manager_t texture_manager;
	
				
} engine_shared_data_t;