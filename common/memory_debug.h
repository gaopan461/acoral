#ifndef _MEMORY_DEBUG_H_
#define _MEMORY_DEBUG_H_

#ifdef _DEBUG
#define DEBUG_MEMORY_ALLOCATOR
#endif

struct memory_cookie;

struct memory_debug
{
#ifdef DEBUG_MEMORY_ALLOCATOR
	int _id;
	long _lock;
	int _break_id;
	memory_cookie* _list;
	memory_debug();
	~memory_debug();
#endif
};

class memory : public memory_debug
{
	memory(){}
	void debug_malloc(memory_cookie*);
	void debug_free(memory_cookie*);
	static void create_instance();
	static memory* _instance;
	static long _singleton_guard;
	static bool _singleton_destroyed;
public:
	~memory();
	void* malloc(size_t size);
	void free(void* p);
	void set_break(int id);
	static memory& instance();
};

#ifndef DEBUG_MEMORY_ALLOCATOR

inline memory& memory::instance()
{
	return *static_cast<memory*>(0);
}

inline void memory::set_break(int id){}

#else

inline memory& memory::instance()
{
	if(!_instance)
		create_instance();

	return *_instance;
}

#endif

#endif //_MEMORY_DEBUG_H_
