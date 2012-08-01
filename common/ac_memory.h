#ifndef _AC_MEMORY_H_
#define _AC_MEMORY_H_

#include "ac_def.h"
namespace acoral
{

	class Memory
	{
	public:
		enum 
		{
			CST_CHUNK_LIMIT		= 16384,
			CST_MAX_NUMBER		= 64,
			CST_ALIGN_SIZE		= 8,
			CST_CHUNK_NUMBER	= CST_CHUNK_LIMIT/CST_ALIGN_SIZE,
		};
	private:
		struct SMemoryList
		{
			SMemoryList* m_pNext;
		};
		struct SChunkList
		{
			SChunkList* m_pNext;
			SMemoryList* m_pData;
		};
		SMemoryList* m_vtFreeList[CST_CHUNK_NUMBER];
		long m_vtGuard[CST_CHUNK_NUMBER];
		SChunkList* m_pChunkList;
		long m_nChunkGuard;
		static Memory* m_pInstance;
		static long m_nSingletonGuard;
		static bool m_bSingletonDestroy;
		static void CreateInstance();
		static size_t ChunkIndex(size_t bytes)
		{
			size_t idx = (bytes - 1) / CST_ALIGN_SIZE;
			assert(idx >= 0 && idx < CST_CHUNK_NUMBER);
			return idx;
		}

		Memory();
		SMemoryList* AllocChunk(size_t idx);
	public:
		~Memory();
		static Memory& Instance()
		{
			if (!m_pInstance)
			{
				CreateInstance();
			}
			return *m_pInstance;
		}
		void* allocate(size_t size);
		void deallocate(void* p, size_t size);
	};

	struct MemoryAlloc
	{
		static void* operator new(size_t size)
		{
			return Memory::Instance().allocate(size);
		}

		static void operator delete(void* p, size_t size)
		{
			Memory::Instance().deallocate(p,size);
		}
	};

}

#endif //_AC_MEMORY_H_