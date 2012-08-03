#ifndef _AC_MEMORY_DEBUG_H_
#define _AC_MEMORY_DEBUG_H_

#include "ac_def.h"

namespace acoral
{

	struct SMemoryCookie;

	struct MemoryDebug
	{
#ifdef AC_DEBUG_MEMORY_ALLOCATOR
		int m_nId;				//记入第一次分配内存，每次分配增1
		long m_nLock;			//链表锁
		int m_nBreakId;			//记录第几次分配断下程序
		SMemoryCookie* m_pList;	//分配的内存信息块链表
		MemoryDebug();
		~MemoryDebug();
#endif
	};

	struct SMemoryDebugCookie
	{
#ifdef AC_DEBUG_MEMORY_ALLOCATOR
		//内存信息块链表为双向链表
		SMemoryCookie* m_pPrev;
		SMemoryCookie* m_pNext;
		int m_nId;				//分配内存的ID（内存为第几次分配）
#endif
	};

	struct SMemoryCookie : public SMemoryDebugCookie
	{
		size_t m_nSize;			//分配的内存大小（包括SMemoryCookie+实际大小）
	};

	class Memory : public MemoryDebug
	{
		Memory(){}

		/*
		 *	将分配的内存信息块存入链表
		 */
		void DebugMalloc(SMemoryCookie*);

		/*
		 *	将释放的内存信息块从链表移除
		 */
		void DebugFree(SMemoryCookie*);
		static void CreateInstance();
		static Memory* m_pInstance;
		static long m_nSingletonGuard;
		static bool m_bSingletonDestroyed;
	public:
		~Memory();
		void* malloc(size_t size);
		void free(void* p);

		/*
		 *	设置在第几次内存分配时断下程序
		 */
		void SetBreak(int id);
		static Memory& Instance();
	};

#ifdef AC_DEBUG_MEMORY_ALLOCATOR

	inline Memory& Memory::Instance()
	{
		if(!m_pInstance)
			CreateInstance();

		return *m_pInstance;
	}

#else

	inline Memory& Memory::Instance()
	{
		return *static_cast<Memory*>(0);
	}

#endif

}

#endif //_AC_MEMORY_DEBUG_H_
