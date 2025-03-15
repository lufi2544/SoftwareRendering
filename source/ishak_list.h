/* date = March 15th 2025 7:10 pm */

///// LINKED LIST IMPLEMENTATION /////

typedef struct list_node_t
{
	void *data;
	struct list_node_t *next_sibling;
	
} list_node_t;

typedef struct
{		
	list_node_t *head;
	u32 size;
} list_t;


