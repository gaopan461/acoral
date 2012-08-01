#include "ac_def.h"
#include "ac_log.h"
#include "ac_memory.h"

using namespace acoral;

class A : public MemoryAlloc
{
public:
	A(int n){m_nNum = n;}
	int Get() const {return m_nNum;}
private:
	int m_nNum;
};

int main()
{
	int a = 100;
	Log::Instance().printf("hello world, a = %d\n",a);

	A* pa = new A(10);
	Log::Instance().printf("A pa = %d\n", pa->Get());
	return 0;
}

