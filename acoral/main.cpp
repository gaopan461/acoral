#include "ac_def.h"
#include "ac_log.h"
#include "ac_memory.h"

class A : public acutils::SmallMemory
{
public:
	A(int n){m_nNum = n;}
	int Get() const {return m_nNum;}
private:
	int m_nNum;
};

#define TEST

namespace acutils
{
	int func()
	{
#ifdef TEST
		INFO_MSG("define test\n");
#else
		INFO_MSG("not define test\n");
#endif
		return 0;
	}
}

void LogInit()
{
	acutils::Log::Instance().AddFacility(new acutils::LogFacilityFile("./test.txt"));
	acutils::Log::Instance().AddFacility(new acutils::LogFacilityConsole());
}

int main()
{
	LogInit();
	INFO_MSG("-------------------------------------------\n");
	INFO_MSG("                Main Start                 \n");
	INFO_MSG("-------------------------------------------\n");
	int a = 100;
	INFO_MSG("hello world, a = %d\n",a);

	A* pa = new A(10);
	INFO_MSG("A pa = %d\n", pa->Get());
	acutils::func();

	DEBUG_MSG("This is a debug message,a = %d\n",a);
	ERROR_MSG("This is a error message,a = %d\n",a);
	WARN_MSG("This is a warning message,a = %d\n",a);
	INFO_MSG("This is a information message,a = %d\n",a);
	return 0;
}


