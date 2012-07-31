#ifndef _SMALLOBJECT_ALLOCATOR_H_
#define _SMALLOBJECT_ALLOCATOR_H_

#include <cassert>

class thread_guard
{
public:
	thread_guard(long*);
	~thread_guard();
private:
	long* _mutex;
};

class smallobject_allocator
{
public:
	enum {
		chunk_limit = 16384,
		max_number = 64,
		align_size = 8,
		chunk_number = chunk_limit/align_size,
	};
private:
	struct memory_list
	{
		memory_list* _next;
	};
	struct chunk_list
	{
		chunk_list* _next;
		memory_list* _data;
	};
	memory_list* _free_list[chunk_number];
	long _guard[chunk_number];
	chunk_list* _chunk_list;
	long _chunk_guard;
	static smallobject_allocator* _instance;
	static long _singleton_guard;
	static bool _singleton_destroy;
	static void create_instance();
	static size_t chunk_index(size_t bytes)
	{
		size_t idx = (bytes - 1) / align_size;
		assert(idx >= 0 && idx < chunk_number);
		return idx;
	}

	smallobject_allocator();
	memory_list* alloc_chunk(size_t idx);
public:
	~smallobject_allocator();
	static smallobject_allocator& instance()
	{
		if (!_instance)
		{
			create_instance();
		}
		return *_instance;
	}
	void* allocate(size_t size);
	void deallocate(void* p, size_t size);
};

struct small_alloc
{
	static void* operator new(size_t size)
	{
		return smallobject_allocator::instance().allocate(size);
	}

	static void operator delete(void* p, size_t size)
	{
		smallobject_allocator::instance().deallocate(p,size);
	}
};

#endif //_SMALLOBJECT_ALLOCATOR_H_