/* date = March 15th 2025 7:10 pm */

///// LINKED LIST IMPLEMENTATION /////

typedef struct list_node_t
{
	void *data;
	u32 index;
	struct list_node_t *next_sibling;
	
} list_node_t;

typedef struct
{		
	list_node_t *head;
	u32 size;
} list_t;


internal list_node_t*
list_add_element(memory_arena_t *_arena, list_t *_list, void* _data, u32 _size);

internal list_t 
make_list(memory_arena_t *_arena, void* _data);


#define LIST_ADD(a, l, d, t) list_add_element(a, &l, &d, sizeof(t))
#define LIST(a) make_list(a, 0)