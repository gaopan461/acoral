#ifndef _AC_MEMORY_DEBUG_H_
#define _AC_MEMORY_DEBUG_H_

#include "ac_def.h"

namespace acutils
{

	struct SMemoryCookie;

	struct MemoryDebug
	{
#ifdef AC_DEBUG_MEMORY_ALLOCATOR
		int m_nId;				//�����һ�η����ڴ棬ÿ�η�����1
		long m_nLock;			//������
		int m_nBreakId;			//��¼�ڼ��η�����³���
		SMemoryCookie* m_pList;	//������ڴ���Ϣ������
		MemoryDebug();
		~MemoryDebug();
#endif
	};

	struct SMemoryDebugCookie
	{
#ifdef AC_DEBUG_MEMORY_ALLOCATOR
		//�ڴ���Ϣ������Ϊ˫������
		SMemoryCookie* m_pPrev;
		SMemoryCookie* m_pNext;
		int m_nId;				//�����ڴ��ID���ڴ�Ϊ�ڼ��η��䣩
#endif
	};

	struct SMemoryCookie : public SMemoryDebugCookie
	{
		size_t m_nSize;			//������ڴ��С������SMemoryCookie+ʵ�ʴ�С��
	};

	class Memory : public MemoryDebug
	{
		Memory(){}

		/*
		 *	��������ڴ���Ϣ���������
		 */
		void DebugMalloc(SMemoryCookie*);

		/*
		 *	���ͷŵ��ڴ���Ϣ��������Ƴ�
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
		 *	�����ڵڼ����ڴ����ʱ���³���
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
