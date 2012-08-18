#ifndef _AC_MEMORY_H_
#define _AC_MEMORY_H_

#include "ac_def.h"
namespace acutils
{

	/*
	 *	С�ڴ������
	 *  ����1������ܷ�����ڴ��СΪ16KB�������ã���������ʹ������ֹ
	 *  ����2�������ڴ���СΪ8�ֽڣ�����8�ֽڶ��루�����ã���СΪ�����ֳ�����һ��ָ��Ĵ�С��
	 */
	class SmallMemoryAlloc
	{
	public:
		enum 
		{
			CST_CHUNK_LIMIT		= 16384,	//һ��chunk���Ϊ16KB
			CST_MAX_NUMBER		= 64,		//һ��chunk����ж��ٶ���
			CST_ALIGN_SIZE		= 8,		//����Ķ����С
			CST_CHUNK_NUMBER	= CST_CHUNK_LIMIT/CST_ALIGN_SIZE,	//�ܹ��ж����ܲ�ͬ��chunk
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

		SMemoryList* m_vtFreeList[CST_CHUNK_NUMBER];	//ÿ��chunk�Ŀ�������
		long m_vtGuard[CST_CHUNK_NUMBER];				//ÿ��chunk����
		SChunkList* m_pChunkList;						//�����ѷ���chunk������
		long m_nChunkGuard;								//ȫ��chunk��
		static SmallMemoryAlloc* m_pInstance;
		static long m_nSingletonGuard;
		static bool m_bSingletonDestroy;
		static void CreateInstance();

		/*
		 *	ͨ��Ҫ����Ĵ�С��ȡchunk��index��chunk����ID��
		 */
		static size_t ChunkIndex(size_t bytes)
		{
			size_t idx = (bytes - 1) / CST_ALIGN_SIZE;
			ACCHECK(idx >= 0 && idx < CST_CHUNK_NUMBER);
			return idx;
		}

		/*
		 *	���캯��˽����δʵ�֣���֤����ʵ�����ܱ�new�������Ӷ���֤����
		 */
		SmallMemoryAlloc();

		/*
		 *	����һ��ָ�����͵�chunk
		 */
		SMemoryList* AllocChunk(size_t idx);
	public:
		~SmallMemoryAlloc();
		static SmallMemoryAlloc& Instance()
		{
			if (!m_pInstance)
			{
				CreateInstance();
			}
			return *m_pInstance;
		}

		/*
		 *	����ָ����С���ڴ�
		 */
		void* allocate(size_t size);

		/*
		 *	�ͷ��ڴ�
		 */
		void deallocate(void* p, size_t size);
	};

	/*
	 *  �ڴ������
	 *  ����new��delete������
	 *	��Ҫʹ�ñ��ڴ���������ڼ̳�֮������ʹ�ñ��ڴ������
	 */
	struct SmallMemory
	{
		static void* operator new(size_t size)
		{
			return SmallMemoryAlloc::Instance().allocate(size);
		}

		static void operator delete(void* p, size_t size)
		{
			SmallMemoryAlloc::Instance().deallocate(p,size);
		}
	};

}

#endif //_AC_MEMORY_H_