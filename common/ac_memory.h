#ifndef _AC_MEMORY_H_
#define _AC_MEMORY_H_

#include "ac_def.h"
namespace acoral
{

	/*
	 *	小内存分配器
	 *  限制1：最大能分配的内存大小为16KB（可配置），超过会使程序终止
	 *  限制2：分配内存最小为8字节，并按8字节对齐（可配置，最小为机器字长，即一个指针的大小）
	 */
	class Memory
	{
	public:
		enum 
		{
			CST_CHUNK_LIMIT		= 16384,	//一个chunk最大为16KB
			CST_MAX_NUMBER		= 64,		//一个chunk最多有多少对象
			CST_ALIGN_SIZE		= 8,		//对象的对齐大小
			CST_CHUNK_NUMBER	= CST_CHUNK_LIMIT/CST_ALIGN_SIZE,	//总共有多少总不同的chunk
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

		SMemoryList* m_vtFreeList[CST_CHUNK_NUMBER];	//每种chunk的空闲链表
		long m_vtGuard[CST_CHUNK_NUMBER];				//每种chunk的锁
		SChunkList* m_pChunkList;						//所有已分配chunk的链表
		long m_nChunkGuard;								//全局chunk锁
		static Memory* m_pInstance;
		static long m_nSingletonGuard;
		static bool m_bSingletonDestroy;
		static void CreateInstance();

		/*
		 *	通过要分配的大小获取chunk的index（chunk种类ID）
		 */
		static size_t ChunkIndex(size_t bytes)
		{
			size_t idx = (bytes - 1) / CST_ALIGN_SIZE;
			ACCHECK(idx >= 0 && idx < CST_CHUNK_NUMBER);
			return idx;
		}

		/*
		 *	构造函数私有且未实现，保证该类实例不能被new出来，从而保证单键
		 */
		Memory();

		/*
		 *	分配一个指定类型的chunk
		 */
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

		/*
		 *	分配指定大小的内存
		 */
		void* allocate(size_t size);

		/*
		 *	释放内存
		 */
		void deallocate(void* p, size_t size);
	};

	/*
	 *  内存分配器
	 *  重载new和delete操作符
	 *	需要使用本内存分配器的内继承之，即可使用本内存分配器
	 */
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