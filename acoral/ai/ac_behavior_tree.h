#ifndef _ac_behavior_tree_
#define _ac_behavior_tree_

#include "ac_def.h"

namespace acoral
{
	class BTNode;

	//节点的执行状态
	enum EBTNodeStatus
	{
		BT_NODE_STATUS_READEY,		//准备执行
		BT_NODE_STATUS_EXCUTING,	//正在执行
		BT_NODE_STATUS_FINISH,				//执行结束
	};

	//节点脚本基类
	class BTScript
	{
	public:
		BTScript(BTNode* pOwner):m_pOwner(pOwner){}
	protected:
		BTNode* m_pOwner;
	};

	//节点进入条件脚本基类
	class BTEvaluateScript : public BTScript
	{
	public:
		BTEvaluateScript(BTNode* pOwner):BTScript(pOwner){}
		virtual bool Excute(void* unit) = 0;
	};

	//节点进入脚本基类
	class BTEnterScript : public BTScript
	{
	public:
		BTEnterScript(BTNode* pOwner):BTScript(pOwner){}
		virtual bool Excute(void* unit) = 0;
	};

	//节点执行脚本基类
	class BTExcuteScript : public BTScript
	{
	public:
		BTExcuteScript(BTNode* pOwner):BTScript(pOwner){}
		virtual EBTNodeStatus Excute(void* unit, const std::vector<BTNode*>& vtChilds) = 0;
	};

	//节点退出脚本基类
	class BTExitScript : public BTScript
	{
	public:
		BTExitScript(BTNode* pOwner):BTScript(pOwner){}
		virtual bool Excute(void* unit) = 0;
	};

	//行为树节点
	class BTNode
	{
	public:
		BTNode(BTNode* pParent);
		~BTNode();
		BTNode& AddChild(BTNode* pChild);
		BTNode& SetName(const char* pName);
		BTNode& AddEvaluateScript(BTEvaluateScript* pScript);
		BTNode& AddEnterScript(BTEnterScript* pScript);
		BTNode& AddExcuteScript(BTExcuteScript* pScript);
		BTNode& AddExitScript(BTExitScript* pScript);
		inline sint32 GetActiveChild(){return m_nActiveChild;}
		void SetActiveChild(void* unit, sint32 nActiveChild);
		bool Evaluate(void* unit);
		bool Enter(void* unit);
		EBTNodeStatus Excute(void* unit);
		bool Exit(void* unit);
	private:
		std::vector<BTNode*> m_vtChilds;			//子节点
		BTNode* m_pParent;									//父节点
		std::string m_strName;									//节点名
		BTEvaluateScript* m_pEvaluteScript;	//进入条件脚本
		BTEnterScript* m_pEnterScript;				//进入脚本
		BTExcuteScript* m_pExcuteScript;			//执行脚本
		BTExitScript* m_pExitScript;						//退出脚本
		bool m_bNeedExit;										//是否需要执行退出脚本
		sint32 m_nActiveChild;
	};

	class BTSeletor : public BTExcuteScript
	{
	public:
		BTSeletor(BTNode* pOwner):BTExcuteScript(pOwner){}
		virtual EBTNodeStatus Excute(void* unit, const std::vector<BTNode*>& vtChilds);
	};

}
#endif