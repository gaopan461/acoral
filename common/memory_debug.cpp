#include "memory_debug.h"
#include "smallobject_allocator.h"
#include "log_console.h"
#include <malloc.h>

//------------------------------------

template <int debug>
struct memory_debug_cookie
{
	memory_cookie* last;
	memory_cookie* next;
	int id;
};

template <> struct memory_debug_cookie<0>{};

struct memory_cookie : public memory_debug_cookie<(sizeof(memory)>sizeof(memory_debug_cookie<0>))>
{
	size_t size;
};

#ifndef DEBUG_MEMORY_ALLOCATOR

inline void memory::debug_malloc(memory_cookie*){}
inline void memory::debug_free(memory_cookie*){}

#else

memory_debug::memory_debug()
{
	_id = 0;
	_lock = 0;
	_break_id = -1;
	_list = reinterpret_cast<memory_cookie*>(smallobject_allocator::instance().allocate(sizeof(memory_cookie)));
	_list->id = -1;
	_list->size = 0;
	memory_cookie* temp = reinterpret_cast<memory_cookie*>(smallobject_allocator::instance().allocate(sizeof(memory_cookie)));
	_list->last = _list->next=temp;
	temp->id = -1;
	temp->size = 0;
	temp->last = temp->next = _list;
}

memory_debug::~memory_debug()
{
	smallobject_allocator::instance().deallocate(_list, sizeof(memory_cookie));
	smallobject_allocator::instance().deallocate(_list->last, sizeof(memory_cookie));
}

void memory::set_break(int id)
{
	_break_id = id;
}

void memory::debug_malloc(memory_cookie* mc)
{
	thread_guard guard(&_lock);
	assert(_id!=_break_id);
	mc->id = _id;
	mc->last = _list;
	mc->next = _list->next;
	_list->next->last = mc;
	_list->next = mc;
	++_id;
}

void memory::debug_free(memory_cookie* mc)
{
	thread_guard guard(&_lock);
	assert(mc->id >= 0);
	mc->last->next = mc->next;
	mc->next->last = mc->last;
	mc->id = -1;
}

memory* memory::_instance = 0;
long memory::_singleton_guard = 0;
bool memory::_singleton_destroyed = false;

void memory::create_instance()
{
	thread_guard guard(&_singleton_guard);
	if(_instance)
		return;
	assert(!_singleton_destroyed);
	static memory obj;
	_instance = &obj;
}

memory::~memory()
{
	if (_list->next == _list->last)
	{
		log::instance().printf("No memory leak\n");
	}
	else
	{
		log::instance().printf("Memory leak:\n");
		memory_cookie* iter = _list->next;
		while(iter!=_list->last)
		{
			log::instance().printf("id=%d size=%d\n",iter->id,iter->size-sizeof(memory_cookie));
			iter = iter->next;
		}
	}
	_singleton_destroyed = true;
	_instance = 0;
}

#endif

void* memory::malloc(size_t size)
{
	if(!size)
	{
		return 0;
	}

	memory_cookie* ret;

	size += sizeof(memory_cookie);
	if(size > smallobject_allocator::chunk_limit)
	{
		ret = reinterpret_cast<memory_cookie*>(::malloc(size));
	}
	else
	{
		ret = reinterpret_cast<memory_cookie*>(smallobject_allocator::instance().allocate(size));
	}

	ret->size = size;
	debug_malloc(ret);
	return ret+1;
}

void memory::free(void* p)
{
	if(!p)
	{
		return;
	}

	memory_cookie* block = reinterpret_cast<memory_cookie*>(p)-1;
	debug_free(block);
	if(block->size > smallobject_allocator::chunk_limit)
	{
		::free(block);
	}
	else
	{
		smallobject_allocator::instance().deallocate(block,block->size);
	}
}