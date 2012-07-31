#include "ac_def.h"
#include "ac_log.h"

using namespace acoral;

int main()
{
	int a = 100;
	Log::Instance().printf("hello world, a = %d\n",a);
	return 0;
}

