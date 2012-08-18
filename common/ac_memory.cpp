#include "ac_memory.h"
#include "ac_thread_guard.h"
#include "ac_utils.h"

namespace acutils
{
	SmallMemoryAlloc* SmallMemoryAlloc::m_pInstance = 0;
	long SmallMemoryAlloc::m_nSingletonGuard = 0;
	bool SmallMemoryAlloc::m_bSingletonDestroy =false;

	void SmallMemoryAlloc::CreateInstance()
	{
		ThreadGuard guard(&m_nSingletonGuard);
		if(m_pInstance)
			return;

		ACCHECK(!m_bSingletonDestroy);
		static SmallMemoryAlloc obj;
		m_pInstance = &obj;
	}

	SmallMemoryAlloc::SmallMemoryAlloc()
	{
		m_pChunkList = 0;
		m_nChunkGuard = 0;
		::memset(m_vtFreeList,0,sizeof(m_vtFreeList));
		::memset(m_vtGuard,0,sizeof(m_vtGuard));
	}

	SmallMemoryAlloc::~SmallMemoryAlloc()
	{
		//释放所有已经分配的chunk
		SChunkList* temp = m_pChunkList;
		SChunkList* temp1 = m_pChunkList;
		while(temp)
		{
			temp1 = temp->m_pNext;
			::free(temp->m_pData);
			temp = temp1;
		}

		m_bSingletonDestroy = true;
		m_pInstance = 0;
	}

	SmallMemoryAlloc::SMemoryList* SmallMemoryAlloc::AllocChunk(size_t idx)
	{
		//该种chunk中每个对象的大小
		const size_t nodeSize = (idx + 1) * CST_ALIGN_SIZE;

		//要分配的chunk的大小（不可超过最大chunk大小，chunk内的对象数也不可超过最大对象数）
		const size_t chunkSize = _min(CST_CHUNK_LIMIT / nodeSize * nodeSize,nodeSize * CST_MAX_NUMBER);

		ThreadGuard guard(&m_nChunkGuard);
		SMemoryList* &currentList = m_vtFreeList[idx];
		if(currentList)
			return currentList;

		//实际分配内存
		currentList = reinterpret_cast<SMemoryList*>(::malloc(chunkSize));

		//把分配出的chunk初始化为内存池链表
		SMemoryList* ret = currentList;
		SMemoryList* iter = ret;
		for (size_t i=0; i<=chunkSize-nodeSize*2; i+=nodeSize)
		{
			iter->m_pNext = iter + nodeSize / sizeof(*iter);
			iter = iter->m_pNext;
		}

		iter->m_pNext = 0;

		//返回新分配的chunk的地址
		return ret;
	}

	void* SmallMemoryAlloc::allocate(size_t size)
	{
		size_t idx = ChunkIndex(size);
		ACCHECK(idx < CST_CHUNK_NUMBER);

		//锁指定大小的内存池
		ThreadGuard guard(&m_vtGuard[idx]);

		SMemoryList* &temp = m_vtFreeList[idx];
		if (!temp)
		{
			//内存池空，分配一个新的chunk做内存池
			SMemoryList* newChunk=AllocChunk(idx);
			SChunkList* chunkNode;

			//此处chunkNode也通过Memory分配器分配内存
			if(ChunkIndex(sizeof(SChunkList)) == idx)
			{
				chunkNode = reinterpret_cast<SChunkList *>(temp);
				temp = temp->m_pNext;
			}
			else
			{
				chunkNode = reinterpret_cast<SChunkList *>(allocate(sizeof(SChunkList)));
			}

			ThreadGuard guard(&m_nChunkGuard);

			//将新分配出来的chunk链接到chunk链表表头
			chunkNode->m_pNext = m_pChunkList;
			chunkNode->m_pData = newChunk;
			m_pChunkList = chunkNode;
		}

		//从内存池中取出一个内存
		void* ret = temp;
		temp = temp->m_pNext;
		return ret;
	}

	void SmallMemoryAlloc::deallocate(void* p, size_t size)
	{
		//得到该对象大小对应的内存池
		size_t idx = ChunkIndex(size);
		ACCHECK(idx < CST_CHUNK_NUMBER);

		SMemoryList* freeBlock = reinterpret_cast<SMemoryList *>(p);
		ThreadGuard guard(&m_vtGuard[idx]);

		//释放的对象的内存链接到内存池链表表头
		SMemoryList* &temp = m_vtFreeList[idx];
		freeBlock->m_pNext = temp;
		temp = freeBlock;
	}

}