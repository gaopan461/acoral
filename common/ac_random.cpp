#include "ac_random.h"

namespace acutils
{
	RandomGenerator::RandomGenerator()
	{
#ifdef _WIN32
		m_hProv = NULL;
		if(!CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, 0))
			if(!CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				ACCHECK(m_hProv);
#else
		m_pFile = fopen("/dev/urandom", "r");
		ACCHECK(m_pFile);
#endif
	}

	RandomGenerator::~RandomGenerator()
	{
#ifdef _WIN32
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
#else
		fclose(m_pFile);
#endif
	}

	int64 RandomGenerator::Gen()
	{
#ifdef _WIN32
		if(m_hProv != NULL)
		{
			int64 result = 0;
			if(CryptGenRandom(m_hProv, sizeof(result), (BYTE*)&result))
				return result;
		}
		return ((int64)rand() << 32) + rand();
#else
		int64 result = 0;
		fread(&result, sizeof(result), 1, m_pFile);
		return result;
#endif
	}
}