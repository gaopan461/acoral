#include "ac_memory.h"
#include "ac_thread_guard.h"
#include "ac_utils.h"

namespace acoral
{
	Memory* Memory::m_pInstance = 0;
	long Memory::m_nSingletonGuard = 0;
	bool Memory::m_bSingletonDestroy =false;

	void Memory::CreateInstance()
	{
		ThreadGuard guard(&m_nSingletonGuard);
		if(m_pInstance)
			return;

		assert(!m_bSingletonDestroy);
		static Memory obj;
		m_pInstance = &obj;
	}

	Memory::Memory()
	{
		m_pChunkList = 0;
		m_nChunkGuard = 0;
		::memset(m_vtFreeList,0,sizeof(m_vtFreeList));
		::memset(m_vtGuard,0,sizeof(m_vtGuard));
	}

	Memory::~Memory()
	{
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

	Memory::SMemoryList* Memory::AllocChunk(size_t idx)
	{
		const size_t nodeSize = (idx + 1) * CST_ALIGN_SIZE;
		const size_t chunkSize = _min(CST_CHUNK_LIMIT / nodeSize * nodeSize,nodeSize * CST_MAX_NUMBER);
		ThreadGuard guard(&m_nChunkGuard);
		SMemoryList* &currentList = m_vtFreeList[idx];
		if(currentList)
			return currentList;

		currentList = reinterpret_cast<SMemoryList*>(::malloc(chunkSize));
		SMemoryList* ret = currentList;
		SMemoryList* iter = ret;
		for (size_t i=0; i<chunkSize-nodeSize*2; i+=nodeSize)
		{
			iter->m_pNext = iter + (idx + 1) * CST_ALIGN_SIZE / sizeof(*iter);
			iter = iter->m_pNext;
		}

		iter->m_pNext = 0;

		return ret;
	}

	void* Memory::allocate(size_t size)
	{
		size_t idx = ChunkIndex(size);
		assert(idx < CST_CHUNK_NUMBER);

		ThreadGuard guard(&m_vtGuard[idx]);

		SMemoryList* &temp = m_vtFreeList[idx];
		if (!temp)
		{
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
			chunkNode->m_pNext = m_pChunkList;
			chunkNode->m_pData = newChunk;
			m_pChunkList = chunkNode;
		}

		void* ret = temp;
		temp = temp->m_pNext;
		return ret;
	}

	void Memory::deallocate(void* p, size_t size)
	{
		size_t idx = ChunkIndex(size);
		assert(idx < CST_CHUNK_NUMBER);

		SMemoryList* freeBlock = reinterpret_cast<SMemoryList *>(p);
		ThreadGuard guard(&m_vtGuard[idx]);
		SMemoryList* &temp = m_vtFreeList[idx];
		freeBlock->m_pNext = temp;
		temp = freeBlock;
	}

}