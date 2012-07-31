#include "ac_behavior_script.h"

namespace acoral
{
	bool BTEvalChild1::Excute(void* unit)
	{
		sint32* pn1 = (sint32*)unit;
		return *pn1 >= 0 && *pn1 < 7;
	}

	bool BTEntChild1::Excute(void* unit)
	{
		sint32* pn1 = (sint32*)unit;
		*pn1 = 5;
		std::cout << "child1 enter,n1:"<< (*pn1)<<std::endl;
		return true;
	}

	EBTNodeStatus BTExeChild1::Excute(void* unit, const std::vector<BTNode*>& vtChilds)
	{
		sint32* pn1 = (sint32*)unit;
		if(*pn1 < 10 && *pn1 >= 5)
		{
			*pn1 = *pn1 + 1;
			std::cout << "child1 excute,n1:" <<(*pn1)<<std::endl;
			return BT_NODE_STATUS_EXCUTING;
		}
		else
		{
			m_pOwner->Exit(unit);
			return BT_NODE_STATUS_FINISH;
		}
	}

	bool BTExiChild1::Excute(void* unit)
	{
		sint32* pn1 = (sint32*)unit;
		*pn1 = 20;
		std::cout << "child1 exit,n1:"<< (*pn1) <<std::endl;
		return true;
	}

	bool BTEvalChild3::Excute(void* unit)
	{
		sint32* pn1 = (sint32*)unit;
		return *pn1 >= 20 && *pn1 < 27;
	}

	bool BTEntChild2::Excute(void* unit)
	{
		sint32* pn1 = (sint32*)unit;
		*pn1 = 25;
		std::cout << "child2 enter,n1:"<< (*pn1)<<std::endl;
		return true;
	}

	EBTNodeStatus BTExeChild2::Excute(void* unit, const std::vector<BTNode*>& vtChilds)
	{
		sint32* pn1 = (sint32*)unit;
		if(*pn1 < 30 && *pn1 >= 25)
		{
			*pn1 = *pn1 + 1;
			std::cout << "child2 excute,n1:" <<(*pn1)<<std::endl;
			return BT_NODE_STATUS_EXCUTING;
		}
		else
		{
			m_pOwner->Exit(unit);
			return BT_NODE_STATUS_FINISH;
		}
	}

	bool BTExiChild2::Excute(void* unit)
	{
		sint32* pn1 = (sint32*)unit;
		*pn1 = 0;
		std::cout << "child2 exit,n1:"<< (*pn1) <<std::endl;
		return true;
	}
}