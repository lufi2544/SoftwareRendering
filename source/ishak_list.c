
internal list_t 
make_list(memory_arena_t *_arena, void* _data)
{
	list_t result;	
	result.size = 0;	
	result.head = 0;
	
	if(_data)
	{
		list_node_t *node = PushStruct(_arena, list_node_t);
		
		if(node)
		{
			result.head = node;
			result.size++;
		}
	}
	
	return result;
}


internal void
list_add_element(memory_arena_t *_arena, list_t *_list, void* _data, u32 _size)
{
	if(_data == 0)
	{
		return;
	}
	
	if(_list)
	{
		list_node_t *node = PushStruct(_arena, list_node_t);
		void *node_data = PushStruct(_arena, void*);
		node->data = node_data;
		
		if(node_data == 0)
		{
			return;
		}
		
		// note: (juanes.rayo): Moving this to a .c file to use as raw copy of bytes.
		u8 *data_as_bytes = (u8*)_data;
		u8 *node_data_as_bytes = (u8*)node_data;
		for(u32 i = 0; i < _size; ++i)
		{
			node_data_as_bytes[i] = data_as_bytes[i];
		}
				
		if(node)
		{
			if(!_list->head)
			{
				_list->head = node;
			}
			else
			{
				list_node_t *prev_head = _list->head;
				node->next_sibling = prev_head;
				_list->head = node;
			}
			
			_list->size++;
		}
	}
}

#define LIST_ADD(a, l, d, t) list_add_element(a, l, &d, sizeof(t))
#define LIST(a) make_list(a, 0)