#include "ac_def.h"
#include "ac_log.h"
#include "ac_memory.h"

class A : public acoral::SmallMemory
{
public:
	A(int n){m_nNum = n;}
	int Get() const {return m_nNum;}
private:
	int m_nNum;
};

#define TEST

namespace acoral
{
	int func()
	{
#ifdef TEST
		Log::Instance().printf("define test\n");
#else
		Log::Instance().printf("not define test\n");
#endif
		return 0;
	}
}

int main()
{
	int a = 100;
	acoral::Log::Instance().printf("hello world, a = %d\n",a);

	A* pa = new A(10);
	acoral::Log::Instance().printf("A pa = %d\n", pa->Get());
	acoral::func();

	DEBUG_MSG("This is a debug message,a = %d\n",a);
	ERROR_MSG("This is a error message,a = %d\n",a);
	WARN_MSG("This is a warning message,a = %d\n",a);
	INFO_MSG("This is a information message,a = %d\n",a);
	return 0;
}

