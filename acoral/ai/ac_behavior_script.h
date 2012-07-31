#ifndef _ac_behavior_script_
#define _ac_hehavior_script_

#include "ac_def.h"
#include "ac_behavior_tree.h"

namespace acoral
{
	class BTEvalChild1 : public BTEvaluateScript
	{
	public:
		BTEvalChild1(BTNode* pOwner):BTEvaluateScript(pOwner){}
		virtual bool Excute(void* unit);
	};

	class BTEntChild1 : public BTEnterScript
	{
	public:
		BTEntChild1(BTNode* pOwner):BTEnterScript(pOwner){}
		virtual bool Excute(void* unit);
	};

	class BTExeChild1 : public BTExcuteScript
	{
	public:
		BTExeChild1(BTNode* pOwner):BTExcuteScript(pOwner){}
		virtual EBTNodeStatus Excute(void* unit, const std::vector<BTNode*>& vtChilds);
	};

	class BTExiChild1 : public BTExitScript
	{
	public:
		BTExiChild1(BTNode* pOwner):BTExitScript(pOwner){}
		virtual bool Excute(void* unit);
	};

	class BTEvalChild3 : public BTEvaluateScript
	{
	public:
		BTEvalChild3(BTNode* pOwner):BTEvaluateScript(pOwner){}
		virtual bool Excute(void* unit);
	};

	class BTEntChild2 : public BTEnterScript
	{
	public:
		BTEntChild2(BTNode* pOwner):BTEnterScript(pOwner){}
		virtual bool Excute(void* unit);
	};

	class BTExeChild2 : public BTExcuteScript
	{
	public:
		BTExeChild2(BTNode* pOwner):BTExcuteScript(pOwner){}
		virtual EBTNodeStatus Excute(void* unit, const std::vector<BTNode*>& vtChilds);
	};

	class BTExiChild2 : public BTExitScript
	{
	public:
		BTExiChild2(BTNode* pOwner):BTExitScript(pOwner){}
		virtual bool Excute(void* unit);
	};
}

#endif //_ac_hehavior_script_