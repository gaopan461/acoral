#ifndef _AC_DEF_H_
#define _AC_DEF_H_

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#endif

#include <vector>
#include <string>
#include <assert.h>
#include <iostream>

#define AC_DEBUG
#define AC_LOG

#ifdef AC_DEBUG
#define AC_DEBUG_MEMORY_ALLOCATOR
#endif

#ifdef _WIN32
	typedef signed char sint8;
	typedef unsigned char uint8;
	typedef signed short sint16;
	typedef unsigned short uint16;
	typedef __int32 sint32;
	typedef unsigned __int32 uint32;
	typedef __int64 sint64;
	typedef unsigned __int64 uint64;
#else
	typedef signed char sint8;
	typedef unsigned char uint8;
	typedef signed short sint16;
	typedef unsigned short uint16;
	typedef int32_t sint32;
	typedef uint32_t uint32;
	typedef long long sint64;
	typedef unsigned long long uint64;
#endif

typedef sint32 int32;
typedef sint64 int64;

#define _safe_delete(x) {if(x) {delete x; x = 0;}}
#define _safe_delete_arrary(x) {if(x){delete [] x; x = 0;}

namespace acutils
{
	inline void InnerAssert(const char* _strExp, const char* _strFile, sint32 _nLine)
	{
		std::cerr << "Assert failed:" << _strExp << " in "<< _strFile << " : "<<_nLine<<std::endl;
		*reinterpret_cast<int*>(0) = 0;
	}
}

#ifdef  AC_DEBUG
#define ACCHECK(S) ((S) || (acutils::InnerAssert(#S, __FILE__, __LINE__), 0));
#else
#define ACCHECK(S)
#endif

#endif //_AC_DEF_H_