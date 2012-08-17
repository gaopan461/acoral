#ifndef _AC_RANDOM_H_
#define _AC_RANDOM_H_

#include "ac_def.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinCrypt.h>
#endif

namespace acutils
{
	/*
	 *	Ëæ¼´Êý²úÉúÆ÷
	 */
	class RandomGenerator
	{
	public:
		RandomGenerator();
		~RandomGenerator();
	public:
		int64 Gen();
	private:
#ifdef _WIN32
		HCRYPTPROV m_hProv;
#else
		FILE* m_pFile;
#endif
	};
}

#endif//_AC_RANDOM_H_