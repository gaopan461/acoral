#include "ac_memory_debug.h"
#include "ac_memory.h"
#include "ac_log.h"

namespace acutils
{

#ifndef AC_DEBUG_MEMORY_ALLOCATOR

	inline void Memory::SetBreak(int id){}
	inline void Memory::DebugMalloc(SMemoryCookie*){}
	inline void Memory::DebugFree(SMemoryCookie*){}

#else

	MemoryDebug::MemoryDebug()
	{
		m_nId = 0;
		m_nLock = 0;
		m_nBreakId = -1;
		m_pList = reinterpret_cast<SMemoryCookie*>(SmallMemoryAlloc::Instance().allocate(sizeof(SMemoryCookie)));
		m_pList->m_nId = -1;
		m_pList->m_nSize = 0;
		SMemoryCookie* temp = reinterpret_cast<SMemoryCookie*>(SmallMemoryAlloc::Instance().allocate(sizeof(SMemoryCookie)));
		m_pList->m_pPrev = m_pList->m_pNext = temp;
		temp->m_nId = -1;
		temp->m_nSize = 0;
		temp->m_pPrev = temp->m_pNext = m_pList;
	}

	MemoryDebug::~MemoryDebug()
	{
		SmallMemoryAlloc::Instance().deallocate(m_pList->m_pPrev, sizeof(SMemoryCookie));
		SmallMemoryAlloc::Instance().deallocate(m_pList, sizeof(SMemoryCookie));
	}

	void Memory::SetBreak(int id)
	{
		m_nBreakId = id;
	}

	void Memory::DebugMalloc(SMemoryCookie* mc)
	{
		ThreadGuard guard(&m_nLock);
		ACCHECK(m_nId != m_nBreakId);
		mc->m_nId = m_nId;
		mc->m_pPrev = m_pList;
		mc->m_pNext = m_pList->m_pNext;
		m_pList->m_pNext->m_pPrev = mc;
		m_pList->m_pNext = mc;
		++m_nId;
	}

	void Memory::DebugFree(SMemoryCookie* mc)
	{
		ThreadGuard guard(&m_nLock);
		ACCHECK(mc->m_nId >= 0);
		mc->m_pPrev->m_pNext = mc->m_pNext;
		mc->m_pNext->m_pPrev = mc->m_pPrev;
		mc->m_nId = -1;
	}

	Memory* Memory::m_pInstance = 0;
	long Memory::m_nSingletonGuard = 0;
	bool Memory::m_bSingletonDestroyed = false;

	void Memory::CreateInstance()
	{
		ThreadGuard guard(&m_nSingletonGuard);
		if(m_pInstance)
			return;

		assert(!m_bSingletonDestroyed);
		static Memory obj;
		m_pInstance = &obj;
	}

	Memory::~Memory()
	{
		//程序结束时，检查是否有内存未被释放
		if (m_pList->m_pNext == m_pList->m_pPrev)
		{
			INFO_MSG("No memory leak\n");
		}
		else
		{
			ERROR_MSG("Memory leak:\n");
			SMemoryCookie* iter = m_pList->m_pNext;
			while(iter != m_pList->m_pPrev)
			{
				ERROR_MSG("id=%d size=%d\n", iter->m_nId, iter->m_nSize - sizeof(SMemoryCookie));
				iter = iter->m_pNext;
			}
		}
		m_bSingletonDestroyed = true;
		m_pInstance = 0;
	}

#endif

	void* Memory::malloc(size_t size)
	{
		if(!size)
		{
			return 0;
		}

		SMemoryCookie* ret;

		size += sizeof(SMemoryCookie);
		if(size > SmallMemoryAlloc::CST_CHUNK_LIMIT)
		{
			ret = reinterpret_cast<SMemoryCookie*>(::malloc(size));
		}
		else
		{
			ret = reinterpret_cast<SMemoryCookie*>(SmallMemoryAlloc::Instance().allocate(size));
		}

		ret->m_nSize = size;
		DebugMalloc(ret);
		return ret+1;
	}

	void Memory::free(void* p)
	{
		if(!p)
		{
			return;
		}

		SMemoryCookie* block = reinterpret_cast<SMemoryCookie*>(p) - 1;
		DebugFree(block);
		if(block->m_nSize > SmallMemoryAlloc::CST_CHUNK_LIMIT)
		{
			::free(block);
		}
		else
		{
			SmallMemoryAlloc::Instance().deallocate(block,block->m_nSize);
		}
	}

}